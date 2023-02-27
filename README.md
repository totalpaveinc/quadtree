
# QuadTree

This QuadTree package is built for [libtilegen](https://github.com/totalpaveinc/libtilegen). While it may be useful for other applications,
it's intend is to provide a quad tree for a geographical world extent.

## How to import

We recommend cloning this repo as a git submodule somewhere within your project.
Then using CMake's [add_subdirectory](https://cmake.org/cmake/help/latest/command/add_subdirectory.html)
directive so that it's included as part of your build system, building using
whatever toolchain you're using. Use the `tpquadtree` name for linking.

This package has subdependencies, therefore you must also init them:

```
cd quadtree
git submodule update --init
```

## Releases

Releases are by tags only. We do not build prebuilt binaries for this package.
Tags can be used to pin to a specific version if desired.

# TotalPave Committers

This section is for TotalPave employees.

## Updating Dependencies

Our only dependency is our own [Extent](https://github.com/totalpaveinc/extent) package, which is a git submodule under `/third_party/extent`

To update it:

1.  go into the submodule directory `/third_praty/extent`
2.  `git pull` any changes from upstream.
3.  `git checkout` the tag to pin to.
4.  `cd ..` up one directory.
5.  Now we add the submodule change to the main repo. `git add extent`
6.  Commit: `git commit -m "dep(extent): v<x.y.z>`

## How to make a release

1.  Open `CMakeLists.txt` and change the `VERSION` to match the version you intend to release. Follow [Semantic Versioning](https://semver.org/) rules.
2.  Commit the change.
3.  Tag the commit `v<x.y.z>`.
4.  Push the commit & tags (`git push && git push --tags`)
