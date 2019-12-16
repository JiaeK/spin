let getProjectRoot = () => {
  let root =
    ConfigFile.Project.path
    |> Utils.Sys.get_parent_file
    |> Option.map(~f=Caml.Filename.dirname);

  switch (root) {
  | Some(f) => f
  | None => raise(Errors.CurrentDirectoryNotASpinProject)
  };
};

let getProjectConfig = () => {
  getProjectRoot() |> ConfigFile.Project.parse;
};

let getProjectDoc = () => {
  getProjectRoot() |> ConfigFile.Project.parse_doc;
};

let listGenerators = (source: Source.t) => {
  let localPath = Source.toLocalPath(source);
  let generatorsDir = Utils.Filename.concat(localPath, "generators");

  Utils.Sys.ls_dir(~recursive=false, generatorsDir)
  |> List.filter(~f=el => {
       Utils.Filename.test(
         Utils.Filename.Exists,
         Utils.Filename.concat(el, "spin"),
       )
     });
};

let getGenerator = (name, ~source: Source.t) => {
  let generators = listGenerators(source);
  switch (
    List.find(
      generators,
      ~f=el => {
        let doc = ConfigFile.Generators.parse_doc(el);
        String.equal(doc.name, name);
      },
    )
  ) {
  | Some(v) => v
  | None => raise(Errors.GeneratorDoesNotExist(name))
  };
};

let generateFile = (~destination: string, ~models, source) => {
  let source = Jg_wrapper.from_string(source, ~models);
  let destination = Jg_wrapper.from_string(destination, ~models);

  let data =
    Stdio.In_channel.read_all(source) |> Jg_wrapper.from_string(~models);

  let parent_dir = Utils.Filename.dirname(destination);
  Utils.Filename.mkdir(parent_dir, ~parent=true);
  Stdio.Out_channel.write_all(destination, ~data);
};

let generate = (~useDefaults=false, ~source: Source.t, name) => {
  let generatorPath = getGenerator(name, ~source);
  let projectRoot = getProjectRoot();
  let projectConfig = getProjectConfig();
  let generator =
    ConfigFile.Generators.parse(
      generatorPath,
      ~useDefaults,
      ~models=projectConfig.models,
    );

  let rec loop =
    fun
    | [] => ()
    | [(f: ConfigFile__Generators.file), ...fs] => {
        generateFile(
          Utils.Filename.concat(generatorPath, f.source),
          ~destination=Utils.Filename.concat(projectRoot, f.destination),
          ~models=generator.models,
        );
        loop(fs);
      };

  Console.log(
    <Pastel>
      <Pastel> "\n🏗️  Creating a new " </Pastel>
      <Pastel color=Pastel.Blue bold=true> {generator.name} </Pastel>
    </Pastel>,
  );
  loop(generator.files);
  Console.log(
    <Pastel color=Pastel.GreenBright bold=true> "Done!\n" </Pastel>,
  );
};
