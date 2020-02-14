# Naming conventions

You might notice that the naming conventions for the Reason syntax used in official Spin templates deviate from common communities practices.

When creating Spin, I wanted to develop a scaffolding tool for both OCaml and Reason. The communities diverge on several aspects, and naming conventions is only one of them. However, I wanted to consider Reason and OCaml as just syntactic differences, so I had to unify a few things and go against some community common practices.

Among these unifications that might feel awkward to some people, we can list:

- Using Rely as a test framework for OCaml
- Using reason-native libraries for OCaml
- Using Esy and Pesy for OCaml

And finally, using snake case syntax for Reason.

I feel like this last point might make people uncomfortable. I understand this very well because it felt uncomfortable to me.

The fact is, however, that changing Reason syntax to snake case seemed like a lesser sin than changing OCaml's syntax to camel case:

- Reason native projects depend heavily on other OCaml dependencies, so having camel case felt inconsistent
- On the same vein, Reason native projects use OCaml's standard library, which uses snake case
- Some Reason projects chose not to follow the camel case conventions, one such example is morph.

Admittedly, I would personally prefer to use camel case, but consistency was more important to me.

Ultimately, nothing is set in stone, and if it appears that using a different naming conventions seems better in the future, nothing is preventing us from changing it!
