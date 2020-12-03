This is a simple `hello world` using object-store `mono-repo`, object-store is a set of classes that facilitates the use of [realm-core](https://github.com/realm/realm-core) which is the API behind the [Realm SDK's](https://github.com/realm).   

### Getting Started

First you need to clone [realm-core](https://github.com/realm/realm-core):

```sh
  git clone https://github.com/realm/realm-core
```
> At the moment of writing this we are in the process of mergin object-store to [realm-core](https://github.com/realm/realm-core), so for now you will need to jump to the `monorepo` branch `git checkout monorepo`.

Then build realm-core, by following [this guide](https://github.com/realm/realm-core/blob/master/how-to-build.md). Please keep in mind that for this (``hello-world``) to work out-of-the-box you need to make sure to name your build folder `build.DEBUG`, otherwise you might need to modify the `Makefile`.


#### This Project

Now you can clone this project:

```sh
  git clone https://github.com/cesarvr/hello-world-realm-core.git
```

Next make a symbolic link of your realm-core folder in this project:

```sh
  ln -s <path-to-your-realm-core folder> realm-core
```

### Running The Example

The hard part should be over, now you just need to run the project:

```sh
make
./a.out
```
> If you get this error `fatal error: 'realm/util/config.h' file not found #include <realm/util/config.h>`, you just need to navigate to the `realm-core` folder and do `cmake .` to generate those files.

You should see this:

```xml
creating folder
 value_one -> one
 value_two -> two
 tracking result_set: 2
```
> The program basically creates a Dict data type and stored in a in-memory database.

### Using XCode 

There is also an XCode project included here so if you prefer, you can open the project with: 

```sh
open xcode/realm-cpp-test/realm-cpp-test.xcodeproj
```


Now you can start hacking 🏴‍☠️ with realm-core.
