# C++与ccache结合使用示例

**Session ID:** ses_099caed10ffeb4yJ4F9OLbdrpo
**Created:** 7/15/2026, 10:36:22 PM
**Updated:** 7/15/2026, 10:51:13 PM

---

## User

c++与ccache的结合使用，给出示例

---

## Assistant (Build · DeepSeek V4 Flash Free · 10.9s)

_Thinking:_

The user is asking about how to use ccache with C++ compilation, with examples. Let me first check if there's a ccache project in the current directory, and then provide a helpful answer about using ccache with C++.

The current directory is `/Volumes/ExtDisk/code/cpp/project/opencodes/ccache` - so this appears to be the ccache source code itself. Let me look at what's here and then provide guidance on using ccache with C++.

Let me first check what's in this directory to understand the context better.

**Tool: read**

**Input:**
```json
{
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/ccache"
}
```

**Output:**
```
<path>/Volumes/ExtDisk/code/cpp/project/opencodes/ccache</path>
<type>directory</type>
<entries>


(0 entries)
</entries>
```

---

## Assistant (Build · DeepSeek V4 Flash Free · 2.3s)

_Thinking:_

The directory is empty. This seems to be just a workspace for the ccache project. Let me provide the user with a comprehensive example of using ccache with C++.

Let me check if ccache is installed on this system.

**Tool: bash**

**Input:**
```json
{
  "command": "which ccache && ccache --version"
}
```

**Output:**
```
ccache not found

```

---

## Assistant (Build · DeepSeek V4 Flash Free · 3.8s)

_Thinking:_

ccache is not installed. Let me provide a comprehensive guide anyway.

Let me search for how to install ccache on macOS.

**Tool: websearch**

**Input:**
```json
{
  "query": "ccache c++ usage examples compilation cache"
}
```

**Output:**
```
Title: ccache(1)
URL: https://ccache.dev/manual/4.9.html
Published: N/A
Author: N/A
Highlights:
ccache - a fast C/C++ compiler cache
...
```
ccache [ccache options]
ccache [KEY=VALUE …​] compiler [compiler options]
compiler [compiler options]
...
The first form takes options described in Command line options below. The second form invokes the compiler, optionally using configuration options as KEY=VALUE arguments. In the third form, ccache is masquerading as the compiler as described in Run modes.
...
Ccache is a compiler cache. It speeds up recompilation by caching the result of previous compilations and detecting when the same compilation is being done again.
...
There are two different ways to use ccache to cache a compilation:
...
Prefix your compilation command with`ccache`. This method is most convenient if you just want to try out ccache or wish to use it for some specific projects. Example:
...
```
ccache gcc -c example.c
```
...
Let ccache masquerade as the compiler. This method is most useful when you wish to use ccache for all your compilations. To do this, create a symbolic link to ccache named as the compiler. For example, here is set up ccache to masquerade as`gcc` and`g++`:
...
```
cp ccache /usr/local/bin/
ln -s ccache /usr/local/bin/gcc
ln -s ccache /usr/local/bin/g++
```
...
On platforms that don’t support symbolic links you can simply copy ccache to the compiler name instead for a similar effect:
...
```
cp ccache /usr/local/bin/gcc
cp ccache /usr/local/bin/g++
```
...
And so forth. This will work as long as the directory with symbolic links or ccache copies comes before the directory with the compiler (typically`/usr/bin`) in`PATH`.
...
These command line options only apply when you invoke ccache as “ccache”. When ccache masquerades as a compiler (as described in the previous section), the normal compiler options apply and you should refer to the compiler’s documentation.
...
```
ccache debug=true compiler_check="%compiler% --version" gcc -c example.c
...
ccache gcc
...
compiler (CCACHE_COMPILER or (deprecated) CCACHE_CC)
...
## How ccache works
...
The basic idea is to detect when you are compiling exactly the same code a second time and reuse the previously produced output. The detection is done by hashing different kinds of information that should be unique for the compilation and then using the hash sum to identify the cached output. Ccache uses BLAKE3, a very fast cryptographic hash algorithm, for the hashing. On a cache hit, ccache is able to supply all of the correct compiler outputs (including all warnings, dependency file, etc) from the cache. Data stored in the cache is checksummed with XXH3, an extremely fast non-cryptographic algorithm, to detect corruption.
...
## C++ modules

---

Title: ccache manual
URL: https://ccache.dev/manual/4.13.6.html
Published: 2026-05-04T00:00:00.000Z
Author: N/A
Highlights:
Ccache is a compiler cache that speeds up recompilation by storing the results of previous compilations and reusing them when the same ...

---

Title: doc/manual.adoc at master · ccache/ccache
URL: https://github.com/ccache/ccache/blob/master/doc/manual.adoc
Published: N/A
Author: N/A
Highlights:
ccache - a fast C/C++ compiler cache
...
Ccache is a compiler cache that speeds up recompilation by storing the results
of previous compilations and reusing them when the same compilation is performed
again.
...
There are two ways to use ccache to cache compilations:
...
1. *Prefix method*: Add `ccache` before your compilation command. This is the
   simplest approach when you want to try ccache or use it for specific
   projects. Example:
...
-------------------------------------------------------------------------------
ccache gcc -c example.c
-------------------------------------------------------------------------------
...
2. *Masquerade method*: Make ccache appear as the compiler by creating a
   symbolic link to ccache with the compiler's name. This is useful when you
   want to use ccache for all compilations. To set up ccache to masquerade as
   `gcc` and `g++`:
...
+
-------------------------------------------------------------------------------
cp ccache /usr/local/bin/
ln -s ccache /usr/local/bin/gcc
ln -s ccache /usr/local/bin/g++
-------------------------------------------------------------------------------
...
On systems that don't support symbolic links, you can copy ccache instead:
...
+
-------------------------------------------------------------------------------
cp ccache /usr/local/bin/gcc
cp ccache /usr/local/bin/g++
-------------------------------------------------------------------------------
...
This works as long as the directory containing the symbolic links or ccache copies
appears before the real compiler directory (typically `/usr/bin`) in your `PATH`.
...
WARNING: The masquerade method works well but may conflict with other tools that
use the same technique. See _<<Using ccache with other compiler wrappers>>_ for
more information.
...
These command line options apply only when you invoke ccache directly as "`ccache`".
When ccache masquerades as a compiler (as described in the previous section), you
should use the normal compiler options and refer to your compiler's documentation.
...
ccache's behavior
...
processed in the
...
order of
...
1. Command line
...
-------------------------------------------------------------------------------
ccache debug=true compiler_check="%compiler% --version" gcc -c example.c
...
in `/home/alice/project1/build
...
-------------------------------------------------------------------------------
ccache gcc -I/usr/include/example -I/home/alice/project2/include -c /home/alice/project1/src/example.c
...
[#config_compiler]
*compiler* (*CCACH
...
_COMPILER* or (deprecated) *CCACHE_CC*)::

    This
...
to force the name of the compiler to use. If set to
    the empty string (which is the
...
), ccache works it out from the
    command line.
...
modules*::
...
Clang builds using `-fmodules`. +
    *Effect*: Allows caching when C++ modules are used. +
    *Trade-off*: May not detect changes in module internal state.
...
=== File storage backend

---

Title: ccache(1) — Arch manual pages
URL: https://man.archlinux.org/man/ccache.1.en
Published: N/A
Author: N/A
Highlights:
ccache - a fast C/C++ compiler cache
...
```
ccache [ccache options]
ccache [KEY=VALUE ...] compiler [compiler options]
compiler [compiler options]
...
The first form takes options described in COMMAND LINE OPTIONS below. The second form invokes the compiler, optionally using configuration options as KEY=VALUE arguments. In the third form, ccache is masquerading as the compiler as described in RUN MODES.
...
Ccache is a compiler cache that speeds up recompilation by storing the results of previous compilations and reusing them when the same compilation is performed again.
...
There are two ways to use ccache to cache compilations:
...
1.Prefix method: Add ccache before your compilation command. This is the simplest approach when you want to try ccache or use it for specific projects. Example:
...
```
ccache gcc -c example.c
```
...
2.Masquerade method: Make ccache appear as the compiler by creating a symbolic link to ccache with the compiler’s name. This is useful when you want to use ccache for all compilations. To set up ccache to masquerade as gcc and g++:
...
```
cp ccache /usr/local/bin/
ln -s ccache /usr/local/bin/gcc
ln -s ccache /usr/local/bin/g++
```
...
On systems that don’t support symbolic links, you can copy ccache instead:
...
```
cp ccache /usr/local/bin/gcc
cp ccache /usr/local/bin/g++
```
...
This works as long as the directory containing the symbolic links or ccache copies appears before the real compiler directory (typically /usr/bin) in your PATH.
...
These command line options apply only when you invoke ccache directly as “ccache”. When ccache masquerades as a compiler (as described in the previous section), you should use the normal compiler options and refer to your compiler’s documentation.
...
```
ccache debug=true compiler_check="%compiler%
...
" gcc -c example
...
in /home/alice/project1/build
...
compiles like this
...
```
ccache gcc -I/usr/include/example -I/home/alice/project2/include -c /home/alice/project1/src/example.c
...
compiler (CCACH
...
_COMPILER or (deprecated) CCACHE_CC)
...
# HOW CCACHE WORKS
...
Ccache detects when you’re compiling the same code and reuses previously stored output. It works by creating a unique hash (the “input hash”) from various information that affects the compilation. When the same hash is encountered again, ccache can supply all the correct compiler outputs from the cache.
...
•Preprocessor mode: Ccache
...
•Direct mode: Ccache hashes the source code and include files directly.

---

Title: Ubuntu Manpage: ccache - a fast C/C++ compiler cache
URL: https://manpages.ubuntu.com/manpages/stonking/man1/ccache.1.html
Published: N/A
Author: Canonical
Highlights:
Ubuntu Manpage: ccache - a fast C/C++ compiler cache
...
a fast C/C++ compiler cache
...
```
ccache [ccache options]
ccache [KEY=VALUE ...] compiler [compiler options]
compiler [compiler options]
...
. The second
...
the compiler,
...
as KEY=VALUE
...
as the compiler
...
Ccache is a compiler cache that speeds up recompilation by storing the results of previous compilations and reusing them when the same compilation is performed again.
...
## RUN MODES
...
There are two ways to use ccache to cache compilations:
...
1.Prefix method: Add ccache before your compilation command. This is the simplest approach when you want to try ccache or use it for specific projects. Example:
...
```
ccache gcc -c example.c
```
...
2.Masquerade method: Make ccache appear as the compiler by creating a symbolic link to ccache with the compiler’s name. This is useful when you want to use ccache for all compilations. To set up ccache to masquerade as gcc and g++:
...
```
ln -s /usr/bin/ccache /usr/local/bin/gcc
ln -s /usr/bin/ccache /usr/local/bin/g++
```
...
To do this on a Debian system, it’s easiest to just prepend /usr/lib/ccache to your PATH. /usr/lib/ccache contains symlinks for all compilers currently installed as Debian packages.
...
On systems that don’t support symbolic links, you can copy ccache instead:
...
```
cp ccache /usr/local/bin/gcc
cp ccache /usr/local/bin/g++
```
...
This works as long as the directory containing the symbolic links or ccache copies appears before the real compiler directory (typically /usr/bin) in your PATH.
...
These command line options apply only when you invoke ccache directly as “ccache”. When ccache masquerades as a compiler (as described in the previous section), you should use the normal compiler options and refer to your compiler’s documentation.
...
```
ccache debug=
...
compiler_check="%compiler% --
...
in /home/alice/project1/build
...
```
ccache gcc -I/usr/include/example -I/home/alice/project2/include -c /home/alice/project1/src/example.c
...
compiler (CCACHE_COMPILER or (deprecated) CCACHE_CC)
...
(CCACHE
...
## HOW CCACHE WORKS
...
Ccache detects when you’re compiling the same code and reuses previously stored output. It works by creating a unique hash (the “input hash”) from various information that affects the compilation. When the same hash is encountered again, ccache can supply all the correct compiler outputs from the cache.
...
•Preprocessor mode
...
•Direct mode: Ccache hashes the source code and include files directly.
...
compilation. If
...
The paths and hash

---

Title: ccache — Debian unstable
URL: https://manpages.debian.org/unstable/ccache/ccache.1.en.html
Published: 2026-05-04T00:00:00.000Z
Author: N/A
Highlights:
Ccache is a compiler cache that speeds up recompilation by storing the results of previous compilations and reusing them when the same ...

---

Title: ccache(1)
URL: https://ccache.dev/manual/4.4.2.html
Published: N/A
Author: N/A
Highlights:
ccache - a fast C/C++ compiler cache
...
```
ccache [options]
...
ccache compiler [compiler options]
compiler [compiler options]                   (via symbolic link)
...
Ccache is a compiler cache. It speeds up recompilation by caching the result of previous compilations and detecting when the same compilation is being done again.
...
There are two ways to use ccache. You can either prefix your compilation commands with`ccache` or you can let ccache masquerade as the compiler by creating a symbolic link (named as the compiler) to ccache. The first method is most convenient if you just want to try out ccache or wish to use it for some specific projects. The second method is most useful for when you wish to use ccache for all your compilations.
...
To use the first method, just make sure that`ccache` is in your`PATH`.
...
To use the symlinks method, do something like this:
...
```
cp ccache /usr/local/bin/
ln -s ccache /usr/local/bin/gcc
ln -s ccache /usr/local/bin/g++
ln -s ccache /usr/local/bin/cc
ln -s ccache /usr/local/bin/c++
```
...
And so forth. This will work as long as the directory with symlinks comes before the path to the compiler (which is usually in`/usr/bin`). After installing you may wish to run “which gcc” to make sure that the correct link is being used.
...
These command line options only apply when you invoke ccache as “ccache”. When invoked as a compiler (via a symlink as described in the previous section), the normal compiler options apply and you should refer to the compiler’s documentation.
...
-c, --cleanup
...
ccache usually
...
command line options
...
`--cc
...
ccache gcc
...
I/usr/include/example -I/home/alice/project2/include
...
c /home/alice/project1/src/example.c
...
compiler (CCACHE_COMPILER or (deprecated) CCACHE_CC)
...
prefix_command (CCACHE
...
## How ccache works
...
The basic idea is to detect when you are compiling exactly the same code a second time and reuse the previously produced output. The detection is done by hashing different kinds of information that should be unique for the compilation and then using the hash sum to identify the cached output. Ccache uses BLAKE3, a very fast cryptographic hash algorithm, for the hashing. On a cache hit, ccache is able to supply all of the correct compiler outputs (including all warnings, dependency file, etc) from the cache. Data stored in the cache is checksummed with XXH3, an extremely fast non-cryptographic algorithm, to detect corruption.
...
Ccache has
...
ways of gathering information used to look up results in the cache:
...
## C++ modules
...
Ccache has support for Clang’s`-fmodules` option. In practice ccache only additionally hashes`module.modulemap` files; it does not know how Clang handles its cached binary form of modules so those are ignored. This should not matter in practice: as long as everything else (including
...
map` files
...
same the cached result
...
work. Still, you
...
to modules to allow caching.
...
## Sharing a cache
...
a cache on
...
## Using ccache with other compiler wrappers
...
The recommended way of combining ccache with another compiler wrapper (such as “distcc”) is by letting ccache execute the compiler wrapper. This is accomplished by defining prefix_command, for example by setting the environment variable`CCACHE_PREFIX` to the name of the wrapper (e.g.`distcc`). Ccache will then prefix the command line with the specified command when running the compiler. To specify several prefix commands, set prefix_command to a colon-separated list of commands.

---

Title: ccache(1) — Arch manual pages
URL: https://man.archlinux.org/man/extra/ccache/ccache.1.en
Published: N/A
Author: N/A
Highlights:
ccache - a fast C/C++ compiler cache
...
```
ccache [ccache options]
ccache [KEY=VALUE ...] compiler [compiler options]
compiler [compiler options]
...
options described in COMMAND LINE OPTIONS below. The second
...
invokes the compiler, optionally using configuration options as KEY=VALUE arguments. In the third
...
, ccache is masquerading as the compiler as described
...
RUN MODES.
...
Ccache is a compiler cache that speeds up recompilation by storing the results of previous compilations and reusing them when the same compilation is performed again.
...
There are two ways to use ccache to cache compilations:
...
1.Prefix method: Add ccache before your compilation command. This is the simplest approach when you want to try ccache or use it for specific projects. Example:
...
```
ccache gcc -c example.c
```
...
2.Masquerade method: Make ccache appear as the compiler by creating a symbolic link to ccache with the compiler’s name. This is useful when you want to use ccache for all compilations. To set up ccache to masquerade as gcc and g++:
...
```
cp ccache /usr/local/bin/
ln -s ccache /usr/local/bin/gcc
ln -s ccache /usr/local/bin/g++
```
...
On systems that don’t support symbolic links, you can copy ccache instead:
...
```
cp ccache /usr/local/bin/gcc
cp ccache /usr/local/bin/g++
```
...
This works as long as the directory containing the symbolic links or ccache copies appears before the real compiler directory (typically /usr/bin) in your PATH.
...
These command line options apply only when you invoke ccache directly as “ccache”. When ccache masquerades as a compiler (as described in the previous section), you should use the normal compiler options and refer to your compiler’s documentation.
...
```
ccache
...
=true compiler_
...
="%compiler%
...
Alice works in /home/alice/project1/build
...
compiles like this:
...
```
ccache gcc -I/usr/include/example -I/home/alice/project2/include -c /home/alice/project1/src/example.c
...
COMPILER or (
...
) CCACHE_CC)
...
# HOW CCACHE WORKS
...
Ccache detects when you’re compiling the same code and reuses previously stored output. It works by creating a unique hash (the “input hash”) from various information that affects the compilation. When the same hash is encountered again, ccache can supply all the correct compiler outputs from the cache.
...
•Preprocessor mode
...
processor on the
...
•Direct mode: Ccache hashes the source code and include files directly.
```

---

## Assistant (Build · DeepSeek V4 Flash Free · 6.9s)

_Thinking:_

Now I have good information about ccache. Let me provide a comprehensive example for using ccache with C++.

## ccache 与 C++ 结合使用

ccache 是一个编译器缓存工具，通过缓存之前的编译结果来加速重复编译。当前系统未安装 ccache，以下是使用示例：

### 安装

```bash
# macOS
brew install ccache

# Ubuntu/Debian
apt install ccache

# 从源码构建（当前空目录即为 ccache 源码目录）
```

### 两种使用方式

**方式一：前缀模式** — 在编译命令前加 `ccache`

```bash
ccache g++ -c main.cpp -o main.o
ccache g++ main.o -o main
```

CMake 中启用：

```bash
cmake -DCMAKE_CXX_COMPILER_LAUNCHER=ccache -B build
# 或
export CMAKE_CXX_COMPILER_LAUNCHER=ccache
cmake -B build
```

**方式二：符号链接伪装模式** — 让 ccache 伪装成编译器

```bash
ln -s /usr/local/bin/ccache /usr/local/bin/g++
# 确保 /usr/local/bin 在 PATH 中优先于 /usr/bin
export PATH=/usr/local/bin:$PATH
# 之后所有 g++ 调用都会经过 ccache
g++ -c main.cpp -o main.o   # 实际由 ccache 处理
```

### 完整示例

```cpp
// main.cpp
#include <iostream>
int main() { std::cout << "Hello, ccache!\n"; }
```

```bash
# 第一次编译 - 缓存未命中，实际编译
ccache g++ main.cpp -o main
# 第二次编译 - 缓存命中，跳过编译
ccache g++ main.cpp -o main

# 查看统计信息
ccache -s
```

### 常用配置

```bash
# 设置缓存大小限制
ccache --max-size=10G

# 设置缓存目录
export CCACHE_DIR=~/.ccache

# 显示统计信息
ccache -s

# 清除缓存
ccache --clear
```

### CMake 项目集成（推荐）

```bash
# 方法1：命令行传参
cmake -DCMAKE_CXX_COMPILER_LAUNCHER=ccache -B build

# 方法2：CMakePresets.json
{
  "configurePresets": [{
    "name": "default",
    "cacheVariables": {
      "CMAKE_CXX_COMPILER_LAUNCHER": "ccache"
    }
  }]
}

# 构建（第二次起大幅加速）
cmake --build build
```

核心原理：ccache 对源文件、头文件内容、编译器选项等计算哈希，相同哈希时直接返回缓存的目标文件，避免重复编译。

---

## User

ccache原理介绍，给出验证的例子

---

## Assistant (Build · DeepSeek V4 Flash Free · 6.5s)

_Thinking:_

The user wants me to explain the principles of how ccache works, and provide verification examples to demonstrate it. Let me look at the ccache source code to understand its internals better, since we're in a ccache source directory. But the directory is empty.

Let me fetch the ccache manual or documentation to get detailed information about how ccache works internally, then create some practical verification examples.

Let me look at the official documentation for the technical details, and then create a demo.

**Tool: bash**

**Input:**
```json
{
  "command": "which ccache 2>/dev/null && ccache --version || echo \"ccache not installed\""
}
```

**Output:**
```
/opt/homebrew/bin/ccache
ccache version 4.13.6
Features: crsh-storage file-storage http-storage redis+unix-storage redis-storage remote-storage

Copyright (C) 2002-2007 Andrew Tridgell
Copyright (C) 2009-2026 Joel Rosdahl and other contributors

See <https://ccache.dev/credits.html> for a complete list of contributors.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 3 of the License, or (at your option) any later
version.

```

**Tool: bash**

**Input:**
```json
{
  "command": "which g++ && g++ --version | head -1"
}
```

**Output:**
```
/usr/bin/g++
Apple clang version 21.0.0 (clang-2100.0.123.102)

```

---

## Assistant (Build · DeepSeek V4 Flash Free · 11.4s)

_Thinking:_

Good, ccache is installed. Let me also check some cache-related environment variables, look at the ccache source code structure, and then create a verification example. Actually, let me look at the ccache source code since we're in the ccache project directory — but it's empty. Let me check if there's a way to look at how ccache determines cache hits/misses.

Let me create a comprehensive verification example that demonstrates:
1. How ccache caches compilation results
2. How changes to source files affect cache hits/misses
3. How changes to include files affect cache hits/misses  
4. How changes to compiler flags affect cache hits/misses
5. What the cache statistics show

Let me also look at some ccache documentation online for more details on the internal algorithm.

**Tool: bash**

**Input:**
```json
{
  "command": "ccache -s"
}
```

**Output:**
```
Local storage:
  Cache size (GiB): 0.0 / 5.0 ( 0.00%)

```

**Tool: webfetch**

**Input:**
```json
{
  "url": "https://ccache.dev/manual/4.13.html#_how_ccache_works",
  "format": "markdown"
}
```

**Output:**
```
    ccache(1)  

# ccache(1)

version 4.13

Table of Contents

-   [Name](#_name)
-   [Synopsis](#_synopsis)
-   [Description](#_description)
-   [Run modes](#_run_modes)
-   [Command line options](#_command_line_options)
    -   [Common options](#_common_options)
    -   [Options for remote file-based storage](#_options_for_remote_file_based_storage)
    -   [Options for scripting or debugging](#_options_for_scripting_or_debugging)
    -   [Extra options](#_extra_options)
-   [Configuration](#_configuration)
    -   [Cache-specific configuration file](#_cache_specific_configuration_file)
    -   [Directory-specific configuration file](#_directory_specific_configuration_file)
    -   [Configuration value syntax](#_configuration_value_syntax)
    -   [Configuration file syntax](#_configuration_file_syntax)
    -   [Boolean values](#_boolean_values)
    -   [Configuration options](#_configuration_options)
    -   [Disabling ccache](#_disabling_ccache)
-   [Remote storage backends](#_remote_storage_backends)
    -   [Storage helper process](#_storage_helper_process)
    -   [Configuration syntax](#_configuration_syntax)
    -   [Storage interaction](#_storage_interaction)
    -   [File storage backend](#_file_storage_backend)
    -   [CRSH storage backend](#_crsh_storage_backend)
    -   [HTTP storage backend](#_http_storage_backend)
    -   [Redis storage backend](#_redis_storage_backend)
-   [Cache size management](#_cache_size_management)
    -   [Automatic cleanup](#_automatic_cleanup)
    -   [Manual cleanup](#_manual_cleanup)
-   [Cache compression](#_cache_compression)
-   [Cache statistics](#_cache_statistics)
-   [How ccache works](#_how_ccache_works)
    -   [Common hashed information](#_common_hashed_information)
    -   [The preprocessor mode](#_the_preprocessor_mode)
    -   [The direct mode](#_the_direct_mode)
    -   [The depend mode](#_the_depend_mode)
-   [Handling of newly created source files](#_handling_of_newly_created_source_files)
-   [Cache debugging](#_cache_debugging)
-   [Compiling in different directories](#_compiling_in_different_directories)
-   [Precompiled headers](#_precompiled_headers)
-   [C++ modules](#_c_modules)
-   [Sharing a local cache](#_sharing_a_local_cache)
-   [Sharing a cache on NFS](#_sharing_a_cache_on_nfs)
-   [Using ccache with other compiler wrappers](#_using_ccache_with_other_compiler_wrappers)
-   [Caveats](#_caveats)
-   [Troubleshooting](#_troubleshooting)
    -   [General](#_general)
    -   [Performance](#_performance)
    -   [Corrupt object files](#_corrupt_object_files)
-   [More information](#_more_information)
-   [Author](#_author)

## [](#_name)Name

ccache - a fast C/C++ compiler cache

## [](#_synopsis)Synopsis

**ccache** \[*ccache options*\]
**ccache** \[*KEY*\=*VALUE* …​\] *compiler* \[*compiler options*\]
*compiler* \[*compiler options*\]

The first form takes options described in [Command line options](#_command_line_options) below. The second form invokes the compiler, optionally using [configuration options](#_configuration) as *KEY*\=*VALUE* arguments. In the third form, ccache is masquerading as the compiler as described in [Run modes](#_run_modes).

## [](#_description)Description

Ccache is a compiler cache that speeds up recompilation by storing the results of previous compilations and reusing them when the same compilation is performed again.

Ccache is designed to produce exactly the same compiler output as a normal compilation. The only difference you should notice is faster build times. Any known exceptions to this behavior are documented in the *[Caveats](#_caveats)* section. If you find a case where ccache produces different output than expected, please report it to us.

## [](#_run_modes)Run modes

There are two ways to use ccache to cache compilations:

1.  **Prefix method**: Add `ccache` before your compilation command. This is the simplest approach when you want to try ccache or use it for specific projects. Example:
    
    ccache gcc -c example.c
    
2.  **Masquerade method**: Make ccache appear as the compiler by creating a symbolic link to ccache with the compiler’s name. This is useful when you want to use ccache for all compilations. To set up ccache to masquerade as `gcc` and `g++`:
    
    cp ccache /usr/local/bin/
    ln -s ccache /usr/local/bin/gcc
    ln -s ccache /usr/local/bin/g++
    
    On systems that don’t support symbolic links, you can copy ccache instead:
    
    cp ccache /usr/local/bin/gcc
    cp ccache /usr/local/bin/g++
    
    This works as long as the directory containing the symbolic links or ccache copies appears before the real compiler directory (typically `/usr/bin`) in your `PATH`.
    
    The masquerade method works well but may conflict with other tools that use the same technique. See *[Using ccache with other compiler wrappers](#_using_ccache_with_other_compiler_wrappers)* for more information.
    

## [](#_command_line_options)Command line options

These command line options apply only when you invoke ccache directly as “ccache”. When ccache masquerades as a compiler (as described in the previous section), you should use the normal compiler options and refer to your compiler’s documentation.

### [](#_common_options)Common options

**\-c**, **\--cleanup**

Clean up the cache by removing not recently used cached files until the specified file number and cache size limits are not exceeded. This also recalculates the cache file count and size totals. Normally, there is no need to initiate cleanup manually as ccache keeps the cache below the specified limits at runtime and keeps statistics up to date on each compilation. Forcing a cleanup is mostly useful if you have modified the cache contents manually or believe that the cache size statistics may be inaccurate.

**\-C**, **\--clear**

Clear the entire cache, removing all cached files, but keeping the configuration file.

**\--config-path** *PATH*

Let the command line options operate on configuration file *PATH* instead of the default. Using this option has the same effect as setting (overriding) the environment variable `CCACHE_CONFIGPATH` temporarily.

**\-d**, **\--dir** *PATH*

Let the command line options operate on cache directory *PATH* instead of the default. For example, to show statistics for a cache directory at `/shared/ccache` you can run `ccache -d /shared/ccache -s`. Using this option has the same effect as setting the environment variable `CCACHE_DIR` temporarily.

**\--evict-namespace** *NAMESPACE*

Remove files created in the given [**namespace**](#config_namespace) from the cache.

**\--evict-older-than** *AGE*

Remove files used less recently than *AGE* from the cache. *AGE* should be an unsigned integer with a `d` (days) or `s` (seconds) suffix. If combined with `--evict-namespace`, only remove files within that namespace.

**\-h**, **\--help**

Print a summary of command line options.

**\-F** *NUM*, **\--max-files** *NUM*

Set the maximum number of files allowed in the cache to *NUM*. Use 0 for no limit. The value is stored in a configuration file in the cache directory and applies to all future compilations.

**\-M** *SIZE*, **\--max-size** *SIZE*

Set the maximum size of the files stored in the cache. *SIZE* should be a number followed by an optional suffix: kB, MB, GB, TB (decimal), KiB, MiB, GiB or TiB (binary). The default suffix is GiB. Use 0 for no limit. The value is stored in a configuration file in the cache directory and applies to all future compilations.

**\-X** *LEVEL*, **\--recompress** *LEVEL*

Recompress the cache to level *LEVEL* using the Zstandard algorithm. The level can be an integer, with the same semantics as the [**compression\_level**](#config_compression_level) configuration option, or the special value **uncompressed** for no compression. See *[Cache compression](#_cache_compression)* for more information. This can potentially take a long time since all files in the cache need to be visited. Only files that are currently compressed with a different level than *LEVEL* will be recompressed.

**\-o** *KEY=VALUE*, **\--set-config** *KEY*\=*VALUE*

Set configuration option *KEY* to *VALUE* in the configuration file. See *[Configuration](#_configuration)* for more information.

**\-x**, **\--show-compression**

Print cache compression statistics. See *[Cache compression](#_cache_compression)* for more information. This can potentially take a long time since all files in the cache need to be visited.

**\-p**, **\--show-config**

Print current configuration options and from where they originate (environment variable, configuration file or compile-time default) in human-readable format.

**\--show-log-stats**

Print statistics counters from the stats log in human-readable format. See [**stats\_log**](#config_stats_log). Use `-v`/`--verbose` once or twice for more details.

**\-s**, **\--show-stats**

Print a summary of configuration and statistics counters in human-readable format. Use `-v`/`--verbose` once or twice for more details. See [Cache statistics](#_cache_statistics) for more information.

**\--stop-storage-helpers**

Storage helpers normally stop after a period of client inactivity, but this option can be used to stop any running helpers immediately.

**\--threads** *THREADS*

Use up to *THREADS* threads for threaded operations. The default is to use one thread per CPU.

**\-v**, **\--verbose**

Increase verbosity. The option can be given multiple times.

**\-V**, **\--version**

Print version and copyright information.

**\-z**, **\--zero-stats**

Zero the cache statistics (but not the configuration options).

### [](#_options_for_remote_file_based_storage)Options for remote file-based storage

**\--trim-dir** *PATH*

Remove not recently used files from directory *PATH* until it is at most the size specified by `--trim-max-size`.

Don’t use this option to trim the local cache. To trim the local cache directory to a certain size, use `CCACHE_MAXSIZE=*SIZE* ccache -c`.

**\--trim-max-size** *SIZE*

Specify the maximum size for `--trim-dir`. *SIZE* should be a number followed by an optional suffix: kB, MB, GB, TB (decimal), KiB, MiB, GiB or TiB (binary). The default suffix is GiB. Use 0 for no limit.

**\--trim-method** *METHOD*

Specify the method to trim a directory with `--trim-dir`. Possible values are:

**atime**

LRU (least recently used) using the file access timestamp. This is the default.

**mtime**

LRU (least recently used) using the file modification timestamp.

**\--trim-recompress** *LEVEL*

Recompress to level *LEVEL* using the Zstandard algorithm when using `--trim-dir`. The level can be an integer, with the same semantics as the [**compression\_level**](#config_compression_level) configuration option, or the special value **uncompressed** for no compression. See *[Cache compression](#_cache_compression)* for more information. This can potentially take a long time since all files in the cache need to be visited. Only files that are currently compressed with a different level than *LEVEL* will be recompressed.

### [](#_options_for_scripting_or_debugging)Options for scripting or debugging

**\--checksum-file** *PATH*

Print the checksum (128 bit XXH3) of the file at *PATH* (`-` for standard input).

**\--extract-result** *PATH*

Extract data stored in the result file at *PATH* (`-` for standard input). The data will be written to `ccache-result.*` files in to the current working directory. This option is only useful when debugging ccache and its behavior.

**\--format** *FORMAT*

Specify format for `--print-log-stats` and `--print-stats`. Possible values are:

**tab**

Tab separated. This is the default.

**json**

JSON formatted.

**\-k** *KEY*, **\--get-config** *KEY*

Print the value of configuration option *KEY*. See *[Configuration](#_configuration)* for more information.

**\--hash-file** *PATH*

Print the hash (160 bit BLAKE3) of the file at *PATH* (`-` for standard input). This is only useful when debugging ccache and its behavior.

**\--inspect** *PATH*

Print the content of a result or manifest file at *PATH* (`-` for standard input) to standard output in human-readable format. File content embedded in a result file will however not be printed; use `--extract-result` to extract the file content. This option is only useful when debugging ccache and its behavior.

**\--print-log-stats**

Print statistics counters from the stats log in machine-parsable (tab-separated or JSON) format. See [**stats\_log**](#config_stats_log) and `--format`.

**\--print-stats**

Print statistics counter IDs and corresponding values in machine-parsable (tab-separated or JSON) format. See `--format`.

**\--print-version**

Print version and don’t do anything else.

### [](#_extra_options)Extra options

When run as a compiler, ccache usually just takes the same command line options as the compiler you are using. The only exception to this is the option `--ccache-skip`. That option can be used to tell ccache to avoid interpreting the next option in any way and to pass it along to the compiler as-is.

`--ccache-skip` currently only tells ccache not to interpret the next option as a special compiler option — the option will still be included in the direct mode hash.

The reason this can be important is that ccache does need to parse the command line and determine what is an input filename and what is a compiler option, as it needs the input filename to determine the name of the resulting object file (among other things). The heuristic ccache uses when parsing the command line is that any argument that exists as a file is treated as an input file name. By using `--ccache-skip` you can force an option to not be treated as an input file name and instead be passed along to the compiler as a command line option.

Another case where `--ccache-skip` can be useful is if ccache interprets an option specially but shouldn’t, since the option has another meaning for your compiler than what ccache thinks.

See also [**ignore\_options**](#config_ignore_options).

## [](#_configuration)Configuration

You can customize ccache’s behavior using configuration files and environment variables. Configuration options are processed in the following order of priority (highest first):

1.  Command line settings in *KEY*\=*VALUE* form. Example:
    
    ccache debug=true compiler\_check="%compiler% --version" gcc -c example.c
    
2.  Environment variables starting with `CCACHE_`.
    
3.  A [directory-specific configuration file](#_directory_specific_configuration_file).
    
4.  The [cache-specific configuration file](#_cache_specific_configuration_file).
    
5.  The system-wide configuration file `<sysconfdir>/ccache.conf` (typically `/etc/ccache.conf` or `/usr/local/etc/ccache.conf`).
    
6.  Compile-time defaults.
    

Exception: If the environment variable `CCACHE_CONFIGPATH` is set, it specifies the only configuration file that will be read (environment variables and command line settings still apply).

### [](#_cache_specific_configuration_file)Cache-specific configuration file

The location of the cache-specific configuration file is determined like this on non-Windows systems:

1.  If `CCACHE_CONFIGPATH` is set, use that path.
    
2.  Otherwise, if the environment variable `CCACHE_DIR` is set then use `$CCACHE_DIR/ccache.conf`.
    
3.  Otherwise, if [**cache\_dir**](#config_cache_dir) is set in the system configuration file then use `<cache_dir>/ccache.conf`.
    
4.  Otherwise, if there is a legacy `$HOME/.ccache` directory then use `$HOME/.ccache/ccache.conf`.
    
5.  Otherwise, if `XDG_CONFIG_HOME` is set then use `$XDG_CONFIG_HOME/ccache/ccache.conf`.
    
6.  Otherwise, use `$HOME/Library/Preferences/ccache/ccache.conf` (macOS) or `$HOME/.config/ccache/ccache.conf` (other systems).
    

On Windows, this is the method used to find the configuration file:

1.  If `CCACHE_CONFIGPATH` is set, use that path.
    
2.  Otherwise, if the environment variable `CCACHE_DIR` is set then use `%CCACHE_DIR%/ccache.conf`.
    
3.  Otherwise, if [**cache\_dir**](#config_cache_dir) is set in the system configuration file then use `<cache_dir>\ccache.conf`. The system-wide configuration on Windows is `%ALLUSERSPROFILE%\ccache\ccache.conf` by default. The `ALLUSERSPROFILE` environment variable is usually `C:\ProgramData`.
    
4.  Otherwise, if there is a legacy `%USERPROFILE%\.ccache` directory then use `%USERPROFILE%\.ccache\ccache.conf`.
    
5.  Otherwise, use `%LOCALAPPDATA%\ccache\ccache.conf` if it exists.
    
6.  Otherwise, use `%APPDATA%\ccache\ccache.conf`.
    

See also the [**cache\_dir**](#config_cache_dir) configuration option for how the cache directory location is determined.

### [](#_directory_specific_configuration_file)Directory-specific configuration file

Ccache searches for a `ccache.conf` file (separate from the cache-specific configuration file described above) in the current working directory or any parent directory. The found file must be owned by the effective user and must not be world-writable; otherwise ccache will abort with an error. The search stops when reaching:

-   a directory listed in a ["ceiling directory"](#config_ceiling_dirs) (by default the user’s home directory)
    
-   a directory that contains a ["ceiling marker"](#config_ceiling_markers) (by default `.git`)
    
-   a directory owned by another user
    
-   a file system boundary (mount point)
    

By default, the file cannot set "unsafe options", i.e. those that affect which commands to execute, which files to write (except cache entries) and which remote storage to use. It is possible to allow unsafe options by adding an entry to [**safe\_dirs**](#config_safe_dirs).

### [](#_configuration_value_syntax)Configuration value syntax

All configuration values support expansion of environment variables. The syntax is similar to POSIX shell syntax: `$VAR` or `${VAR}`. Both variants will expand to the value of the environment variable `VAR`.

Two consecutive dollar signs (`$$`) will expand to a single dollar sign (`$`).

### [](#_configuration_file_syntax)Configuration file syntax

Configuration files are in a simple “key = value” format, one option per line. Lines starting with a hash sign are comments. Blank lines are ignored, as is whitespace surrounding keys and values. Example:

\# Set maximum cache size to 10 GB:
max\_size = 10GB

#### [](#_multi_line_values)Multi-line values

Values can span multiple lines using indentation-based continuation. Lines that start with whitespace (spaces or tabs) are treated as continuation lines and are joined to the previous value with a single space. Comments and blank lines within a multi-line value are skipped. For example:

ignore\_options =
  -Wall
  -Wextra
  # This is a comment within the value
  -pedantic
# This ends the multi-line value
compiler = gcc

This is equivalent to:

ignore\_options = -Wall -Wextra -pedantic
compiler = gcc

Multi-line values are particularly useful for options that accept multiple items.

### [](#_boolean_values)Boolean values

Some configuration options are boolean values (i.e. truth values). In a configuration file, such values must be set to the string **true** or **false**. For the corresponding environment variables, the semantics are a bit different:

-   A set environment variable means “true” (even if set to the empty string).
    
-   The following case-insensitive negative values are considered an error (instead of surprising the user): **0**, **false**, **disable** and **no**.
    
-   An unset environment variable means “false”.
    

Each boolean environment variable also has a negated form starting with `CCACHE_NO`. For example, `CCACHE_COMPRESS` can be set to force compression and `CCACHE_NOCOMPRESS` can be set to force no compression.

### [](#_configuration_options)Configuration options

Below is a list of available configuration options. The corresponding environment variable name is indicated in parentheses after each configuration option key.

Options that define a list of paths have their entries separated by `;` on Windows and `:` on other systems.

**absolute\_paths\_in\_stderr** (**CCACHE\_ABSSTDERR**)

This option specifies whether ccache should rewrite relative paths in the compiler’s textual output (standard error and standard output) to absolute paths. This can be useful if you use [**base\_dir**](#config_base_dir) with a build system (e.g. CMake with the "Unix Makefiles" generator) that executes the compiler in a different working directory, which makes relative paths in compiler errors or warnings incorrect. The default is false.

**base\_dir** (**CCACHE\_BASEDIR**)

This option specifies a list of absolute paths. When set, ccache converts absolute paths to relative paths before hashing, but only for paths that start with one of the specified base directories.

This enables cache sharing between compilations in different directories, even when the project uses absolute paths. See [Compiling in different directories](#_compiling_in_different_directories)\_ for more details. When empty (the default), no path rewriting occurs.

Avoid using `/` as the base directory as this will also rewrite system header paths, which is usually counterproductive.

Example scenario: Alice works in `/home/alice/project1/build` and compiles like this:

ccache gcc -I/usr/include/example -I/home/alice/project2/include -c /home/alice/project1/src/example.c

Here is what ccache will actually execute for different **base\_dir** settings:

\# Current working directory: /home/alice/project1/build

# With base\_dir = /:
gcc -I../../../../usr/include/example -I../../project2/include -c ../src/example.c

# With base\_dir = /home or /home/alice:
gcc -I/usr/include/example -I../../project2/include -c ../src/example.c

# With base\_dir = /home/alice/project1 or /home/alice/project1/src:
gcc -I/usr/include/example -I/home/alice/project2/include -c ../src/example.c

If Bob stores the same projects in `/home/bob/stuff` and both users set **base\_dir** to `/home` or `/home/$USER`, they will share cache hits because the rewritten command lines will be identical:

\# Current working directory: /home/bob/stuff/project1/build

# With base\_dir = /home or /home/bob:
gcc -I/usr/include/example -I../../project2/include -c ../src/example.c

Without **base\_dir** there will be a cache miss since the absolute paths will differ. With **base\_dir** set to `/` there will be a cache miss since the relative path to `/usr/include/example` will be different. With **base\_dir** set to `/home/bob/stuff/project1` there will a cache miss since the path to project2 will be a different absolute path.

Rewriting absolute paths to relative is kind of a brittle hack. It works OK in many cases, but there might be cases where things break. One known issue is that absolute paths are not reproduced in dependency files, which can mess up dependency detection in tools like Make and Ninja. If possible, use relative paths in the first place instead of using **base\_dir**.

**cache\_dir** (**CCACHE\_DIR**)

This option specifies where ccache will keep its cached compiler outputs.

On non-Windows systems, the default is `$HOME/.ccache` if such a directory exists, otherwise `$XDG_CACHE_HOME/ccache` if `XDG_CACHE_HOME` is set, otherwise `$HOME/Library/Caches/ccache` (macOS) or `$HOME/.cache/ccache` (other systems).

On Windows, the default is `%USERPROFILE%\.ccache` if such a directory exists, otherwise `%LOCALAPPDATA%\ccache`.

Previous ccache versions defaulted to storing the cache in `%APPDATA%\ccache` on Windows. This can result in large network file transfers of the cache in domain environments and similar problems. Please check this directory for cache directories and either delete them or the whole directory, or move them to the `%LOCALAPPDATA%\ccache` directory.

See also *[Cache-specific configuration file](#_cache_specific_configuration_file)*.

**ceiling\_dirs** (**CCACHE\_CEILING\_DIRS**)

This option specifies absolute directories where ccache will stop searching for a [directory-specific configuration file](#_directory_specific_configuration_file). You might want to set this if accessing files in parent directories is too slow, e.g. on network file systems. The default is to stop looking when reaching the user’s home directory.

**ceiling\_markers** (**CCACHE\_CEILING\_MARKERS**)

This option specifies one or more file or directory names (separated by `;` on Windows, `:` on other systems). When searching for a [directory-specific configuration file](#Directory-specific
configuration file), ccache will stop searching parent directories when it reaches a directory containing any of the specified names. The default is `.git`.

**compiler** (**CCACHE\_COMPILER** or (deprecated) **CCACHE\_CC**)

This option can be used to force the name of the compiler to use. If set to the empty string (which is the default), ccache works it out from the command line.

**compiler\_check** (**CCACHE\_COMPILERCHECK**)

By default, ccache includes the modification time (“mtime”) and size of the compiler in the hash to ensure that results retrieved from the cache are accurate. If compiler plugins are used, these plugins will also be added to the hash. This option can be used to select another strategy. Possible values are:

**content**

Hash the content of the compiler binary. This makes ccache very slightly slower compared to **mtime**, but makes it cope better with compiler upgrades during a build bootstrapping process.

**mtime**

Hash the compiler’s mtime and size, which is fast. This is the default.

**none**

Don’t hash anything. This may be good for situations where you can safely use the cached results even though the compiler’s mtime or size has changed (e.g. if the compiler is built as part of your build system and the compiler’s source has not changed, or if the compiler only has changes that don’t affect code generation). You should only use **none** if you know what you are doing.

**string:value**

Hash **value**. This can for instance be a compiler revision number or another string that the build system generates to identify the compiler.

*a command string*

Hash the standard output and standard error output of the specified command. The string will be split on whitespace to find out the command and arguments to run. No other interpretation of the command string will be done, except that the special word **%compiler%** will be replaced with the path to the compiler. Several commands can be specified with `;` as the separator. Examples:

%compiler% -v

%compiler% -dumpmachine; %compiler% -dumpversion

You should make sure that the specified command is as fast as possible since it will be run once for each ccache invocation.

Identifying the compiler using a command is useful if you want to avoid cache misses when the compiler has been rebuilt but not changed.

Another case is when the compiler (as seen by ccache) actually isn’t the real compiler but another compiler wrapper — in that case, the default **mtime** method will hash the mtime and size of the other compiler wrapper, which means that ccache won’t be able to detect a compiler upgrade. Using a suitable command to identify the compiler is thus safer, but it’s also slower, so you should consider continue using the **mtime** method in combination with the **prefix\_command** option if possible. See *[Using ccache with other compiler wrappers](#_using_ccache_with_other_compiler_wrappers)*.

**compiler\_type** (**CCACHE\_COMPILERTYPE**)

Ccache normally guesses the compiler type based on the compiler name. The **compiler\_type** option lets you force a compiler type. This can be useful if the compiler has a non-standard name but is actually one of the known compiler types. Possible values are:

**auto**

Guess one of the types below based on the compiler name (following symlinks). This is the default.

**clang**

Clang-based compiler.

**clang-cl**

clang-cl.

**gcc**

GCC-based compiler.

**icl**

Intel compiler on Windows.

**icx**

Intel LLVM-based compiler.

**icx-cl**

Intel LLVM-based MSVC-compatible compiler.

**msvc**

Microsoft Visual C++ (MSVC).

**nvcc**

NVCC (CUDA) compiler.

**other**

Any compiler other than the known types.

**compression** (**CCACHE\_COMPRESS** or **CCACHE\_NOCOMPRESS**, see *[Boolean values](#_boolean_values)* above)

If true, ccache will compress data it puts in the cache. However, this option has no effect on how files are retrieved from the cache; compressed and uncompressed results will still be usable regardless of this option. The default is true.

Compression is done using the Zstandard algorithm. The algorithm is fast enough that there should be little reason to turn off compression to gain performance. One exception is if the cache is located on a compressed file system, in which case the compression performed by ccache of course is redundant.

Compression will be disabled if file cloning (the [**file\_clone**](#config_file_clone) option) or hard linking (the [**hard\_link**](#config_hard_link) option) is enabled.

**compression\_level** (**CCACHE\_COMPRESSLEVEL**)

This option determines the level at which ccache will compress object files using the real-time compression algorithm Zstandard. It only has effect if [**compression**](#config_compression) is enabled (which it is by default). Zstandard is extremely fast for decompression and very fast for compression for lower compression levels. The default is 0.

Semantics of **compression\_level**:

**\> 0**

A positive value corresponds to normal Zstandard compression levels. Lower levels (e.g. **1**) mean faster compression but worse compression ratio. Higher levels (e.g. **19**) mean slower compression but better compression ratio. The maximum possible value depends on the libzstd version, but at least up to 19 is available for all versions. Decompression speed is essentially the same for all levels. As a rule of thumb, use level 5 or lower since higher levels may slow down compilations noticeably. Higher levels are however useful when recompressing the cache with command line option `-X`/`--recompress`.

**< 0**

A negative value corresponds to Zstandard’s “ultra-fast” compression levels, which are even faster than level 1 but with less good compression ratios. For instance, level **\-3** corresponds to `--fast=3` for the `zstd` command line tool. In practice, there is little use for levels lower than **\-5** or so.

**0** (default)

The value **0** means that ccache will choose a suitable level, currently **1**.

See the [Zstandard documentation](https://facebook.github.io/zstd/) for more information.

**cpp\_extension** (**CCACHE\_EXTENSION**)

This option can be used to force a certain extension for the intermediate preprocessed file. The default is to automatically determine the extension to use for intermediate preprocessor files based on the type of file being compiled, but that sometimes doesn’t work. For example, when using the “aCC” compiler on HP-UX, set the cpp extension to **i**.

**debug** (**CCACHE\_DEBUG** or **CCACHE\_NODEBUG**, see *[Boolean values](#_boolean_values)* above)

If true, enable the debug mode. The debug mode creates per-object debug files that are helpful when debugging unexpected cache misses. Note however that ccache performance will be reduced slightly. See *[Cache debugging](#_cache_debugging)* for more information. The default is false.

**debug\_dir** (**CCACHE\_DEBUGDIR**)

Specifies where to write per-object debug files if the [debug mode](#config_debug) is enabled. If set to the empty string, the files will be written next to the object file. If set to a directory, the debug files will be written with full absolute paths in that directory, creating it if needed. The default is the empty string.

For example, if **debug\_dir** is set to `/example`, the current working directory is `/home/user` and the object file is `build/output.o` then the debug log will be written to `/example/home/user/build/output.o.ccache-log`. See also *[Cache debugging](#_cache_debugging)*.

**debug\_level** (**CCACHE\_DEBUGLEVEL**)

Specifies the amount of information that is written when the [debug mode](#config_debug) is enabled. See *[Cache debugging](#_cache_debugging)* for more information. The default is 2.

**depend\_mode** (**CCACHE\_DEPEND** or **CCACHE\_NODEPEND**, see *[Boolean values](#_boolean_values)* above)

If true, the depend mode will be used. The default is false. See *[The depend mode](#_the_depend_mode)*.

**direct\_mode** (**CCACHE\_DIRECT** or **CCACHE\_NODIRECT**, see *[Boolean values](#_boolean_values)* above)

If true, the direct mode will be used. The default is true. See *[The direct mode](#_the_direct_mode)*.

**disable** (**CCACHE\_DISABLE** or **CCACHE\_NODISABLE**, see *[Boolean values](#_boolean_values)* above)

When true, ccache will just call the real compiler, bypassing the cache completely. The default is false.

It is also possible to disable ccache for a specific source code file by adding the string `ccache:disable` in a comment in the first 4096 bytes of the file.

**extra\_files\_to\_hash** (**CCACHE\_EXTRAFILES**)

This option is a list of paths to files that ccache will include in the hash sum that identifies the build.

**file\_clone** (**CCACHE\_FILECLONE** or **CCACHE\_NOFILECLONE**, see *[Boolean values](#_boolean_values)* above)

If true, ccache will attempt to use file cloning (also known as “copy on write”, “CoW” or “reflinks”) to store and fetch cached compiler results. **file\_clone** has priority over [**hard\_link**](#config_hard_link). The default is false.

Files stored by cloning cannot be compressed, so the cache size will likely be significantly larger if this option is enabled. However, performance may be improved depending on the use case.

Unlike the [**hard\_link**](#config_hard_link) option, **file\_clone** is completely safe to use, but not all file systems support the feature. For such file systems, ccache will fall back to use plain copying (or hard links if [**hard\_link**](#config_hard_link) is enabled).

**hard\_link** (**CCACHE\_HARDLINK** or **CCACHE\_NOHARDLINK**, see *[Boolean values](#_boolean_values)* above)

If true, ccache will attempt to use hard links to store and fetch cached object files. The default is false.

Files stored via hard links cannot be compressed, so the cache size will likely be significantly larger if this option is enabled. However, performance may be improved depending on the use case.

Do not enable this option unless you are aware of these caveats:

-   If the resulting file is modified, the file in the cache will also be modified since they share content, which corrupts the cache entry. As of version 4.0, ccache makes stored and fetched object files read-only as a safety measure. Furthermore, a simple integrity check is made for cached object files by verifying that their sizes are correct. This means that mistakes like `strip file.o` or `echo >file.o` will be detected even if the object file is made writable, but a modification that doesn’t change the file size will not.
    
-   Programs that don’t expect that files from two different identical compilations are hard links to each other can fail.
    
-   Programs that rely on modification times (like `make`) can be confused if several users (or one user with several build trees) use the same cache directory. The reason for this is that the object files share i-nodes and therefore modification times. If `file.o` is in build tree **A** (hard-linked from the cache) and `file.o` then is produced by ccache in build tree **B** by hard-linking from the cache, the modification timestamp will be updated for `file.o` in build tree **A** as well. This can retrigger relinking in build tree **A** even though nothing really has changed.
    

**hash\_dir** (**CCACHE\_HASHDIR** or **CCACHE\_NOHASHDIR**, see *[Boolean values](#_boolean_values)* above)

If true (which is the default), ccache will include the current working directory (CWD) in the hash that is used to distinguish two compilations when generating debug info (compiler option `-g` with variations). Exception: The CWD will not be included in the hash if the compiler options `-fdebug-prefix-map` or `-fdebug-compilation-dir` are used appropriately. See also the discussion under *[Compiling in different directories](#_compiling_in_different_directories)*.

The reason for including the CWD in the hash by default is to prevent a problem with the storage of the current working directory in the debug info of an object file, which can lead ccache to return a cached object file that has the working directory in the debug info set incorrectly.

You can disable this option to get cache hits when compiling the same source code in different directories if you don’t mind that CWD in the debug info might be incorrect.

**ignore\_headers\_in\_manifest** (**CCACHE\_IGNOREHEADERS**)

This option is a list of paths to files (or directories with headers) that ccache will **not** include in the manifest list that makes up the direct mode. Note that this can cause stale cache hits if those headers do indeed change.

**ignore\_options** (**CCACHE\_IGNOREOPTIONS**)

This option is a space-delimited list of compiler options that ccache will ignore. Entries in the list can optionally end with an asterisk (`*`) to matching any option suffix. For example, `-fmessage-length=*` will match both `-fmessage-length=20` and `-fmessage-length=70`. A matching compiler option will neither be interpreted specially nor be part of the input hash. Ignoring a compiler option from the hash can be useful when you know it doesn’t affect the result (and ccache doesn’t know that), or when it does and you don’t care. See also *[Extra options](#_extra_options)*.

**inode\_cache** (**CCACHE\_INODECACHE** or **CCACHE\_NOINODECACHE**, see *[Boolean values](#_boolean_values)* above)

If true, ccache will cache source file hashes based on device, inode and timestamps. This reduces the time spent on hashing include files since the result can be reused between compilations. The default is true. The feature requires [**temporary\_dir**](#config_temporary_dir) to be located on a local filesystem of a supported type.

Support for the inode cache feature on Windows is experimental. On Windows the default is false.

**keep\_comments\_cpp** (**CCACHE\_COMMENTS** or **CCACHE\_NOCOMMENTS**, see *[Boolean values](#_boolean_values)* above)

If true, ccache will not discard the comments before hashing preprocessor output. The default is false. This can be used to check documentation with `-Wdocumentation`.

**libexec\_dirs** (**CCACHE\_LIBEXEC\_DIRS**)

If set, ccache will look for helper programs in these directories instead of the default.

**log\_file** (**CCACHE\_LOGFILE**)

If set to a file path, ccache will write information on what it is doing to the specified file. This is useful for tracking down problems.

If set to **syslog**, ccache will log using `syslog()` instead of to a file. If you use rsyslogd, you can add something like this to `/etc/rsyslog.conf` or a file in `/etc/rsyslog.d`:

\# log ccache to file
:programname, isequal, "ccache"         /var/log/ccache
# remove from syslog
& ~

**max\_files** (**CCACHE\_MAXFILES**)

This option specifies the maximum number of files to keep in the cache. Use 0 for no limit (which is the default). See also *[Cache size management](#_cache_size_management)*.

**max\_size** (**CCACHE\_MAXSIZE**)

This option specifies the maximum size of the cache. Use 0 for no limit. The default value is 5GiB. Available suffixes: kB, MB, GB, TB (decimal) and KiB, MiB, GiB, TiB (binary). The default suffix is GiB. See also *[Cache size management](#_cache_size_management)*.

**msvc\_dep\_prefix** (**CCACHE\_MSVC\_DEP\_PREFIX**)

This option specifies the prefix of included files output for MSVC compiler. The default prefix is “Note: including file:”. If you use a localized compiler, this should be set accordingly.

**msvc\_utf8** (**CCACHE\_MSVC\_UTF8**)

This option adds `/utf-8` to the msvc command line when executing the preprocessor to ensure that filenames are not garbled for non-ascii characters. This implicitly enables `/validate-charset` and treats the source code as utf-8 which may cause compilation errors if comments in your code have characters in the \[128, 255\] range for a given Windows system codepage which results in an invalid utf-8 sequence. The default is true.

**namespace** (**CCACHE\_NAMESPACE**)

If set, the namespace string will be added to the hashed data for each compilation. This will make the associated cache entries logically separate from cache entries with other namespaces, but they will still share the same storage space. Cache entries can also be selectively removed from the local cache with the command line option `--evict-namespace`, potentially in combination with `--evict-older-than`.

For instance, if you use the same local cache for several disparate projects, you can use a unique namespace string for each one. This allows you to remove cache entries that belong to a certain project if you stop working with that project.

**path** (**CCACHE\_PATH**)

If set, ccache will search directories in this list when looking for the real compiler. If not set, ccache will look for the first executable matching the compiler name in the normal `PATH` that isn’t a symbolic link to ccache itself.

**pch\_external\_checksum** (**CCACHE\_PCH\_EXTSUM** or **CCACHE\_NOPCH\_EXTSUM**, see *[Boolean values](#_boolean_values)* above)

When this option is set, and ccache finds a precompiled header file, ccache will look for a file with the extension “.sum” added (e.g. “pre.h.gch.sum”), and if found, it will hash this file instead of the precompiled header itself to work around the performance penalty of hashing very large files.

**prefix\_command** (**CCACHE\_PREFIX**)

This option adds a list of prefixes (separated by space) to the command line that ccache uses when invoking the compiler. See also *[Using ccache with other compiler wrappers](#_using_ccache_with_other_compiler_wrappers)*.

**prefix\_command\_cpp** (**CCACHE\_PREFIX\_CPP**)

This option adds a list of prefixes (separated by space) to the command line that ccache uses when invoking the preprocessor.

**read\_only** (**CCACHE\_READONLY** or **CCACHE\_NOREADONLY**, see *[Boolean values](#_boolean_values)* above)

If true, ccache will attempt to use existing cached results, but it will not add new results to any cache backend. Statistics counters will still be updated, though, unless the [**stats**](#config_stats) option is set to **false**.

If you are using this because your ccache directory is read-only, you need to set [**temporary\_dir**](#config_temporary_dir) since ccache will fail to create temporary files otherwise. You may also want to set [**stats**](#config_stats) to **false** make ccache not even try to update stats files.

**read\_only\_direct** (**CCACHE\_READONLY\_DIRECT** or **CCACHE\_NOREADONLY\_DIRECT**, see *[Boolean values](#_boolean_values)* above)

Just like [**read\_only**](#config_read_only) except that ccache will only try to retrieve results from the cache using the direct mode, not the preprocessor mode. See documentation for [**read\_only**](#config_read_only) regarding using a read-only ccache directory.

**recache** (**CCACHE\_RECACHE** or **CCACHE\_NORECACHE**, see *[Boolean values](#_boolean_values)* above)

If true, ccache will not use any previously stored result. New results will still be cached, possibly overwriting any pre-existing results.

**remote\_only** (**CCACHE\_REMOTE\_ONLY** or **CCACHE\_NOREMOTE\_ONLY**, see *[Boolean values](#_boolean_values)* above)

If true, ccache will only use [remote storage](#config_remote_storage). The default is false. Note that cache statistics counters will still be kept in the local cache directory unless [**stats**](#config_stats) is false. See also *[Storage interaction](#_storage_interaction)*.

**remote\_storage** (**CCACHE\_REMOTE\_STORAGE**)

This option specifies one or several storage backends (separated by whitespace) to query after checking the local cache (unless [**remote\_only**](#config_remote_only) is true). See *[Remote storage backends](#_remote_storage_backends)* for documentation of syntax and available backends.

Examples:

remote\_storage = file:/shared/nfs/directory+\`
remote\_storage =
  file:///shared/nfs/one read-only
  file:///shared/nfs/two
remote\_storage = file:///Z:/example/windows/folder
remote\_storage = http://example.com/cache
remote\_storage = redis://example.com

In previous ccache versions this option was called **secondary\_storage** (**CCACHE\_SECONDARY\_STORAGE**), which can still be used as an alias.

**reshare** (**CCACHE\_RESHARE** or **CCACHE\_NORESHARE**, see *[Boolean values](#_boolean_values)* above)

If true, ccache will write results to remote storage even for local storage cache hits. The default is false.

**response\_file\_format** (**CCACHE\_RESPONSE\_FILE\_FORMAT**)

Ccache normally guesses the response file format based on the compiler type. The **response\_file\_format** option lets you force the response file quoting behavior. This can be useful if the compiler supports both POSIX and Windows response file quoting. Possible values are:

**auto**

Guess one of the formats below based on the compiler type. This is the default.

**posix**

POSIX quoting behavior.

**windows**

Windows quoting behavior.

**safe\_dirs** (**CCACHE\_SAFE\_DIRS**)

List of trusted directories for which directory-specific configuration files are allowed to set "unsafe options" (see [Directory-specific configuration file](#_directory_specific_configuration_file)). The default is empty.

Allowed list values: `*` matches any directory. An absolute path ending with `/*` matches any subdirectory (at any depth) under that path. An absolute path not ending in `/*` matches only that directory.

**sloppiness** (**CCACHE\_SLOPPINESS**)

By default, ccache tries to give as few false cache hits as possible. However, in certain situations it’s possible that you know things that ccache can’t take for granted. This option makes it possible to tell ccache to relax some checks in order to increase the hit rate. Available values (separated by whitespace or comma):

**clang\_index\_store**

**Use case**: Xcode projects with varying index store paths.  
**Effect**: Ignores the `-index-store-path` and `-index-unit-output-path` options when hashing.  
**Trade-off**: Index won’t update correctly on cache hits.

**file\_stat\_matches**

**Use case**: When file content checking is too slow.  
**Effect**: Uses file timestamps instead of content for cache validation.  
**Trade-off**: May miss content changes with identical timestamps.

**file\_stat\_matches\_ctime**

**Use case**: When controlling file timestamps manually.  
**Effect**: Ignores status change time when `file_stat_matches` is enabled.  
**Trade-off**: May miss some file system changes.

**gcno\_cwd**

**Use case**: Code coverage builds in different directories.  
**Effect**: Ignores current directory when creating `.gcno` files (`-ftest-coverage`). Also disables hashing of the current working directory if `-fprofile-abs-path` is used.  
**Trade-off**: Directory information in coverage files may be incorrect.

No effect with `--coverage` (it implies `-fprofile-arcs`).

**incbin**

**Use case**: Projects using assembly `.incbin` directives.  
**Effect**: Allows caching files with `.incbin` directives.  
**Trade-off**: Won’t detect changes to included binary files.

**include\_file\_ctime**

**Use case**: Build systems that modify file timestamps frequently.  
**Effect**: Ignores file status change time when checking for recent modifications.  
**Trade-off**: May miss recent changes to source files.

**include\_file\_mtime**

**Use case**: Build systems that modify file timestamps frequently.  
**Effect**: Ignores file modification time when checking for recent changes.  
**Trade-off**: May miss recent modifications to source files.

**ivfsoverlay**

**Use case**: Xcode projects mixing Objective-C and Swift.  
**Effect**: Ignores `-ivfsoverlay` virtual filesystem option.  
**Trade-off**: May not detect VFS-related changes.

**locale**

**Use case**: Builds in environments with varying locale settings.  
**Effect**: Ignores locale environment variables (`LANG`, `LC_ALL`, `LC_CTYPE`, `LC_MESSAGES`).  
**Trade-off**: Compiler warning messages may vary between cached and fresh builds.

**modules**

**Use case**: Clang builds using `-fmodules`.  
**Effect**: Allows caching when C++ modules are used.  
**Trade-off**: May not detect changes in module internal state.

See *[C++ modules](#_c_modules)* for details.

**pch\_defines**

**Use case**: Projects with precompiled headers.  
**Effect**: Relaxes checking of `#define` directives in precompiled headers.  
**Trade-off**: May not detect some macro definition changes.

See *[Precompiled headers](#_precompiled_headers)* for details.

**random\_seed**

**Use case**: Builds with varying `-frandom-seed` values.  
**Effect**: Ignores random seed values in compilation hash.  
**Trade-off**: Builds may not be fully reproducible.

**system\_headers**

**Use case**: Systems with frequently changing system headers.  
**Effect**: Only tracks non-system headers in direct mode.  
**Trade-off**: Won’t detect system header changes that affect compilation.  
**Limitations**: Only supported for GCC-like compilers (not MSVC). System headers are still checked in preprocessor mode.

See also [**ignore\_headers\_in\_manifest**](#config_ignore_headers_in_manifest).

**time\_macros**

**Use case**: Code using time macros but values don’t matter.  
**Effect**: Ignores `__DATE__`, `__TIME__`, and `__TIMESTAMP__` in source.  
**Trade-off**: Time values in output will be from cached compilation.

**stats** (**CCACHE\_STATS** or **CCACHE\_NOSTATS**, see *[Boolean values](#_boolean_values)* above)

If true, ccache will update the statistics counters on each compilation. The default is true. If false, *[automatic cleanup](#_automatic_cleanup)* will be disabled as well.

**stats\_log** (**CCACHE\_STATSLOG**)

If set to a file path, ccache will write statistics counter updates to the specified file. This is useful for getting statistics for individual builds. To show a summary of the current stats log, use `ccache --show-log-stats`.

Lines in the stats log starting with a hash sign (`#`) are comments.

**temporary\_dir** (**CCACHE\_TEMPDIR**)

This option specifies where ccache will put temporary files. The default is `$XDG_RUNTIME_DIR/ccache-tmp` (typically `/run/user/<UID>/ccache-tmp`) if `XDG_RUNTIME_DIR` is set and the directory exists, otherwise `<cache_dir>/tmp`.

In previous versions of ccache, **CCACHE\_TEMPDIR** had to be on the same filesystem as the `CCACHE_DIR` path, but this requirement has been relaxed.

**umask** (**CCACHE\_UMASK**)


...40255 bytes truncated...

The tool call succeeded but the output was truncated. Full output saved to: /Users/caviar/.local/share/opencode/tool-output/tool_f663a7c9a001Ecv4tAWXvZ6QQG
Use the Task tool to have explore agent process this file with Grep and Read (with offset/limit). Do NOT read the full file yourself - delegate to save context.
```

---

## Assistant (Build · DeepSeek V4 Flash Free · 5.7s)

_Thinking:_

I have the official documentation on how ccache works. Now let me create a verification example. Let me first check the ccache source code structure briefly, then create a comprehensive demo.

Key points about how ccache works:

1. **Direct mode** (default): Hashes source file content + include files + compiler options + compiler binary info. If hash matches, returns cached result directly without running preprocessor.

2. **Preprocessor mode** (fallback): Runs preprocessor, hashes the preprocessed output + compiler options. Used when direct mode misses.

3. **Depend mode**: Similar to direct mode but uses compiler's dependency output to track include files.

The hash includes:
- Source code content
- Include file contents (recursively)
- Compiler options
- Compiler binary (mtime/size by default)
- Current working directory (if generating debug info)
- Environment variables

Let me create a verification example that demonstrates all these aspects.

Now let me look at the source code structure briefly, then create a verification demo.

**Tool: bash**

**Input:**
```json
{
  "command": "ls /Volumes/ExtDisk/code/cpp/project/opencodes/ccache"
}
```

**Output:**
```
(no output)
```

**Tool: bash**

**Input:**
```json
{
  "command": "which g++ && g++ --version | head -1 && echo \"---\" && ccache --version | head -2"
}
```

**Output:**
```
/usr/bin/g++
Apple clang version 21.0.0 (clang-2100.0.123.102)
---
ccache version 4.13.6
Features: crsh-storage file-storage http-storage redis+unix-storage redis-storage remote-storage

```

---

## Assistant (Build · DeepSeek V4 Flash Free · 64.6s)

_Thinking:_

The ccache project directory is empty - this is just a placeholder workspace. Let me create a comprehensive verification demo in a temp directory. I'll create:

1. A simple C++ project
2. Demonstrate cache hit on recompilation
3. Demonstrate cache miss when source changes
4. Demonstrate cache miss when header changes
5. Demonstrate cache miss when compiler flags change
6. Demonstrate cache stats before/after
7. Show internal hash mechanism via debug mode

Let me create the example in a temp directory.

**Tool: bash**

**Input:**
```json
{
  "command": "mkdir -p /tmp/ccache-demo && rm -rf /tmp/ccache-demo/*"
}
```

**Output:**
```
zsh:1: no matches found: /tmp/ccache-demo/*

```

---

## Assistant (Build · DeepSeek V4 Flash Free · 11.6s)

_Thinking:_

That's fine if the directory is empty. Let me proceed with creating the demo files.

**Tool: write**

**Input:**
```json
{
  "filePath": "/tmp/ccache-demo/main.cpp",
  "content": "#include <iostream>\n#include \"math_util.h\"\n\nint main() {\n    int x = 42;\n    std::cout << \"factorial(\" << x << \") = \" << factorial(x) << std::endl;\n    return 0;\n}\n"
}
```

**Output:**
```
Wrote file successfully.
```

**Tool: write**

**Input:**
```json
{
  "filePath": "/tmp/ccache-demo/math_util.h",
  "content": "#ifndef MATH_UTIL_H\n#define MATH_UTIL_H\n\nconstexpr int factorial(int n) {\n    return (n <= 1) ? 1 : n * factorial(n - 1);\n}\n\n#endif\n"
}
```

**Output:**
```
Wrote file successfully.
```

**Tool: write**

**Input:**
```json
{
  "filePath": "/tmp/ccache-demo/demo.sh",
  "content": "#!/bin/bash\n# ccache 原理验证脚本\n# 清空缓存统计\nccache -z\n\necho \"=== 1. 首次编译 (cache miss) ===\"\nccache g++ -std=c++17 -c main.cpp -o main.o\nccache -s | grep -E \"(cache hit|cache miss|calls)\"\necho \"\"\n\necho \"=== 2. 相同代码再次编译 (cache hit) ===\"\nccache g++ -std=c++17 -c main.cpp -o main.o\nccache -s | grep -E \"(cache hit|cache miss|calls)\"\necho \"\"\n\necho \"=== 3. 第三次编译确认命中率 ===\"\nccache g++ -std=c++17 -c main.cpp -o main.o\nccache -s | grep -E \"(cache hit|cache miss|calls)\"\necho \"\"\n\necho \"=== 4. 修改源文件 -> cache miss ===\"\necho '// added comment' >> main.cpp\nccache g++ -std=c++17 -c main.cpp -o main.o\nccache -s | grep -E \"(cache hit|cache miss|calls)\"\necho \"\"\n\necho \"=== 5. 恢复源文件 -> cache hit ===\"\ngit checkout -- main.cpp 2>/dev/null || sed -i '' '$d' main.cpp\nccache g++ -std=c++17 -c main.cpp -o main.o\nccache -s | grep -E \"(cache hit|cache miss|calls)\"\necho \"\"\n\necho \"=== 6. 修改头文件 -> cache miss ===\"\necho '// modified' >> math_util.h\nccache g++ -std=c++17 -c main.cpp -o main.o\nccache -s | grep -E \"(cache hit|cache miss|calls)\"\necho \"\"\n\necho \"=== 7. 修改编译选项 -> cache miss ===\"\ngit checkout -- math_util.h 2>/dev/null || sed -i '' '$d' math_util.h\nccache g++ -std=c++17 -O2 -c main.cpp -o main.o\nccache -s | grep -E \"(cache hit|cache miss|calls)\"\necho \"\"\n\necho \"=== 8. 不同目录编译 (默认hash_dir开启) -> cache miss ===\"\nmkdir -p build_sub\ncp main.cpp build_sub/\nccache g++ -std=c++17 -c build_sub/main.cpp -o build_sub/main.o\nccache -s | grep -E \"(cache hit|cache miss|calls)\"\n"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · DeepSeek V4 Flash Free · 4.8s)

_Thinking:_

Now let me make the script executable and run it, or better yet, let me run the steps interactively to show the user the results step by step.

Actually, let me run the script but first check what the ccache stats counters look like.

Now let me run the verification step by step:

**Tool: bash**

**Input:**
```json
{
  "command": "cd /tmp/ccache-demo && ccache -z && echo \"Cache stats zeroed\""
}
```

**Output:**
```
Statistics zeroed
Cache stats zeroed

```

---

## Assistant (Build · DeepSeek V4 Flash Free · 4.9s)

_Thinking:_

Good. Now let me run through the demo steps one by one.

**Tool: bash**

**Input:**
```json
{
  "command": "cd /tmp/ccache-demo && echo \"=== 第1次编译: cache miss ===\" && ccache g++ -std=c++17 -c main.cpp -o main.o && ccache -s | grep -E \"(cache hit|cache miss|calls|files in cache)\""
}
```

**Output:**
```
=== 第1次编译: cache miss ===
Cacheable calls:      1 /   1 (100.0%)

```

---

## Assistant (Build · DeepSeek V4 Flash Free · 3.2s)

_Thinking:_

Hmm, the output doesn't show "cache miss" explicitly. Let me check the full stats.

**Tool: bash**

**Input:**
```json
{
  "command": "ccache -s"
}
```

**Output:**
```
Cacheable calls:      1 /   1 (100.0%)
  Hits:               0 /   1 ( 0.00%)
    Direct:           0
    Preprocessed:     0
  Misses:             1 /   1 (100.0%)
Local storage:
  Cache size (GiB): 0.0 / 5.0 ( 0.00%)
  Hits:               0 /   1 ( 0.00%)
  Misses:             1 /   1 (100.0%)

```

---

## Assistant (Build · DeepSeek V4 Flash Free · 3.6s)

_Thinking:_

Good, 1 miss, 0 hits. Now let me compile again to show a cache hit.

**Tool: bash**

**Input:**
```json
{
  "command": "cd /tmp/ccache-demo && echo \"=== 第2次编译: cache hit ===\" && ccache g++ -std=c++17 -c main.cpp -o main.o && ccache -s | grep -E \"(Hits:|Misses:|Direct:|Preprocessed:)\""
}
```

**Output:**
```
=== 第2次编译: cache hit ===
  Hits:               1 /   2 (50.00%)
    Direct:           1 /   1 (100.0%)
    Preprocessed:     0 /   1 ( 0.00%)
  Misses:             1 /   2 (50.00%)
  Hits:               1 /   2 (50.00%)
  Misses:             1 /   2 (50.00%)

```

---

## Assistant (Build · DeepSeek V4 Flash Free · 3.8s)

_Thinking:_

Got a direct cache hit on the second compilation. Now let me modify the source file to trigger a cache miss.

**Tool: bash**

**Input:**
```json
{
  "command": "cd /tmp/ccache-demo && echo \"=== 第3次编译: 修改源文件 -> cache miss ===\" && echo \"// extra comment to change source\" >> main.cpp && ccache g++ -std=c++17 -c main.cpp -o main.o && ccache -s | grep -E \"(Hits:|Misses:)\" | head -4"
}
```

**Output:**
```
=== 第3次编译: 修改源文件 -> cache miss ===
  Hits:               2 /   3 (66.67%)
  Misses:             1 /   3 (33.33%)
  Hits:               2 /   3 (66.67%)
  Misses:             1 /   3 (33.33%)

```

---

## Assistant (Build · DeepSeek V4 Flash Free · 41.5s)

_Thinking:_

Hmm, it's still 1 miss, but the total went to 3. Actually, it seems it didn't increment misses. Wait - let me look more carefully. That's showing 2 hits, 1 miss out of 3 calls. But we should have 1 miss (first compile) + 1 hit (second) + 1 miss (third) = 2 misses. Let me check - the source modification might not have changed the semantics enough to cause a new hash.

Wait, I added a comment to the source file. The preprocessor output should include comments by default... Actually, ccache by default strips comments before hashing? Let me check the `keep_comments_cpp` option.

Actually, let me reconsider. ccache's direct mode hashes the source file directly. A comment change should affect the hash. Let me check more carefully.

Actually, looking at the output again - it says "Misses: 1 / 3 (33.33%)" which means 1 miss in 3 calls. But we already had 1 miss from the first compile. The third compile should have been a miss too (source changed). So either the comment change didn't trigger a miss, or the headers/manifest is cached.

Wait, let me re-read the output more carefully. The "Cacheable calls" are 3. We have:
- Hit: 2 (from the second call and now the third call also hit)
- Miss: 1 (first call)

So adding a comment to the source file didn't cause a cache miss! That's because ccache might be using some sloppiness, or... Let me check - did I accidentally keep the same main.cpp content?

Actually wait, the first line shows "Hits: 2 / 3" and "Misses: 1 / 3". If the third call was a miss, we'd see "Misses: 2 / 3". So it was a hit. But we added a comment... 

Hmm, let me think. Maybe the `echo "// comment" >> main.cpp` actually put the file back to its original state? No, it appends. Let me look more carefully at the ccache mechanism.

Actually, I think the issue is that ccache's direct mode tracks include files. But the main source file itself changed. Let me verify the content changed and try again.

Actually, let me just look at what happened and try a more obvious change.

**Tool: bash**

**Input:**
```json
{
  "command": "cd /tmp/ccache-demo && echo \"File changed?\" && tail -3 main.cpp && echo \"---\" && echo \"Modify source more obviously and retry:\" && echo 'int dummy = 999;' >> main.cpp && ccache g++ -std=c++17 -c main.cpp -o main.o && ccache -s | grep -E \"(Hits:|Misses:)\" | head -4"
}
```

**Output:**
```
File changed?
    return 0;
}
// extra comment to change source
---
Modify source more obviously and retry:
  Hits:               2 /   4 (50.00%)
  Misses:             2 /   4 (50.00%)
  Hits:               2 /   4 (50.00%)
  Misses:             2 /   4 (50.00%)

```

---

## Assistant (Build · DeepSeek V4 Flash Free · 4.5s)

_Thinking:_

Now we have 2 misses and 2 hits in 4 calls. The cache miss was triggered by changing the source code with an actual code change. Let me now demonstrate the header change and compiler flag change.

Let me restore the original file first and re-do the proper demo. Let me create a fresh start.

**Tool: bash**

**Input:**
```json
{
  "command": "cd /tmp/ccache-demo && ccache -z && rm -f main.cpp math_util.h && cat > main.cpp << 'EOF'\n#include <iostream>\n#include \"math_util.h\"\n\nint main() {\n    int x = 10;\n    std::cout << \"factorial(\" << x << \") = \" << factorial(x) << std::endl;\n    return 0;\n}\nEOF\ncat > math_util.h << 'EOF'\n#ifndef MATH_UTIL_H\n#define MATH_UTIL_H\n\nconstexpr int factorial(int n) {\n    return (n <= 1) ? 1 : n * factorial(n - 1);\n}\n\n#endif\nEOF\necho \"Files reset to original\""
}
```

**Output:**
```
Statistics zeroed
Files reset to original

```

---

## Assistant (Build · DeepSeek V4 Flash Free · 5.9s)

**Tool: bash**

**Input:**
```json
{
  "command": "cd /tmp/ccache-demo && echo \"=== Test 1: 首次编译 (cache miss) ===\" && ccache g++ -std=c++17 -c main.cpp -o main.o && ccache -s | grep -E \"(Hits:|Misses:|Cacheable)\""
}
```

**Output:**
```
=== Test 1: 首次编译 (cache miss) ===
Cacheable calls:      1 /   1 (100.0%)
  Hits:               0 /   1 ( 0.00%)
  Misses:             1 /   1 (100.0%)
  Hits:               0 /   1 ( 0.00%)
  Misses:             1 /   1 (100.0%)

```

**Tool: bash**

**Input:**
```json
{
  "command": "cd /tmp/ccache-demo && echo \"=== Test 2: 相同代码 (cache hit - direct mode) ===\" && ccache g++ -std=c++17 -c main.cpp -o main.o && ccache -s | grep -E \"(Hits:|Misses:|Direct:|Preprocessed)\""
}
```

**Output:**
```
=== Test 2: 相同代码 (cache hit - direct mode) ===
  Hits:               0 /   2 ( 0.00%)
    Direct:           0
    Preprocessed:     0
  Misses:             2 /   2 (100.0%)
  Hits:               0 /   2 ( 0.00%)
  Misses:             2 /   2 (100.0%)

```

**Tool: bash**

**Input:**
```json
{
  "command": "cd /tmp/ccache-demo && echo \"=== Test 3: 修改源文件 -> cache miss ===\" && sed -i '' 's/x = 10/x = 20/' main.cpp && ccache g++ -std=c++17 -c main.cpp -o main.o && ccache -s | grep -E \"(Hits:|Misses:)\" | head -4"
}
```

**Output:**
```
=== Test 3: 修改源文件 -> cache miss ===
  Hits:               0 /   3 ( 0.00%)
  Misses:             3 /   3 (100.0%)
  Hits:               0 /   3 ( 0.00%)
  Misses:             3 /   3 (100.0%)

```

**Tool: bash**

**Input:**
```json
{
  "command": "cd /tmp/ccache-demo && echo \"=== Test 4: 修改头文件 -> cache miss ===\" && sed -i '' 's/(n <= 1)/(n < 1)/' math_util.h && sed -i '' 's/x = 20/x = 10/' main.cpp && ccache g++ -std=c++17 -c main.cpp -o main.o && ccache -s | grep -E \"(Hits:|Misses:)\" | head -4"
}
```

**Output:**
```
=== Test 4: 修改头文件 -> cache miss ===
  Hits:               0 /   4 ( 0.00%)
  Misses:             4 /   4 (100.0%)
  Hits:               0 /   4 ( 0.00%)
  Misses:             4 /   4 (100.0%)

```

**Tool: bash**

**Input:**
```json
{
  "command": "cd /tmp/ccache-demo && echo \"=== Test 5: 修改编译器选项 (-O2) -> cache miss ===\" && sed -i '' 's/(n < 1)/(n <= 1)/' math_util.h && ccache g++ -std=c++17 -O2 -c main.cpp -o main.o && ccache -s | grep -E \"(Hits:|Misses:)\" | head -4"
}
```

**Output:**
```
=== Test 5: 修改编译器选项 (-O2) -> cache miss ===
  Hits:               0 /   5 ( 0.00%)
  Misses:             5 /   5 (100.0%)
  Hits:               0 /   5 ( 0.00%)
  Misses:             5 /   5 (100.0%)

```

**Tool: bash**

**Input:**
```json
{
  "command": "cd /tmp/ccache-demo && echo \"=== Test 6: 恢复后重新编译 (cache hit) ===\" && ccache g++ -std=c++17 -O2 -c main.cpp -o main.o && ccache -s | grep -E \"(Hits:|Misses:)\" | head -4"
}
```

**Output:**
```
=== Test 6: 恢复后重新编译 (cache hit) ===
  Hits:               0 /   6 ( 0.00%)
  Misses:             6 /   6 (100.0%)
  Hits:               0 /   6 ( 0.00%)
  Misses:             6 /   6 (100.0%)

```

---

## Assistant (Build · DeepSeek V4 Flash Free · 7.0s)

_Thinking:_

Interesting - the tests didn't produce any cache hits at all! Everything is a miss. This means something is off. Let me investigate. The issue might be that the source files are being treated differently. Let me debug this more carefully.

Actually wait - the same source was compiled 3 times (tests 1, 2, 3) but the source changed between them. In test 2, the source should have been the same as test 1, so it should have been a cache hit.

Let me investigate why. Maybe it's because the CWD is being included in the hash (hash_dir is on by default), or maybe the file is being modified between calls.

Actually, I think the issue is that sed -i '' is modifying the file even when it doesn't need to change. Let me check if the files are actually being modified.

Wait, actually looking more carefully: in test 2, we compile the same code again but the stats show 2 misses. That's strange. Let me debug this by checking what ccache is hashing.

Let me run with debug mode to see what's happening.

**Tool: bash**

**Input:**
```json
{
  "command": "cd /tmp/ccache-demo && echo \"Debug: check if original files are consistent\" && md5sum main.cpp math_util.h && echo \"---\" && echo \"Try fresh compile sequence in a clean subdir:\" && mkdir -p clean_test && cp main.cpp math_util.h clean_test/ && cd clean_test && ccache -z && ccache g++ -std=c++17 -c main.cpp -o main.o && ccache -s | grep -E \"(Hits:|Misses:)\" | head -2 && echo \"Second compile (should hit):\" && ccache g++ -std=c++17 -c main.cpp -o main.o && ccache -s | grep -E \"(Hits:|Misses:)\" | head -2"
}
```

**Output:**
```
Debug: check if original files are consistent
5e7bf039aaaa0819820a3f25dc4bdc3b  main.cpp
00573b824e1808b328592b37abe80017  math_util.h
---
Try fresh compile sequence in a clean subdir:
Statistics zeroed
  Hits:               1 /   1 (100.0%)
  Misses:             0 /   1 ( 0.00%)
Second compile (should hit):
  Hits:               2 /   2 (100.0%)
  Misses:             0 /   2 ( 0.00%)

```

---

## Assistant (Build · DeepSeek V4 Flash Free · 5.8s)

_Thinking:_

So in a clean subdirectory, it works perfectly! 100% hits after the first compile. The issue in the previous tests must have been that the source file was being modified between compilations. I made mistakes in the test script.

Let me now redo the proper verification demo in a clean way. I'll create a fresh directory and do the tests properly.

**Tool: bash**

**Input:**
```json
{
  "command": "rm -rf /tmp/ccache-demo && mkdir -p /tmp/ccache-demo && cat > /tmp/ccache-demo/main.cpp << 'EOF'\n#include <iostream>\n#include \"math_util.h\"\nint main() {\n    int x = 10;\n    std::cout << \"factorial(\" << x << \") = \" << factorial(x) << \"\\n\";\n}\nEOF\ncat > /tmp/ccache-demo/math_util.h << 'EOF'\n#ifndef MATH_UTIL_H\n#define MATH_UTIL_H\nconstexpr int factorial(int n) { return (n <= 1) ? 1 : n * factorial(n - 1); }\n#endif\nEOF\nccache -z && echo \"done\""
}
```

**Output:**
```
Statistics zeroed
done

```

---

## Assistant (Build · DeepSeek V4 Flash Free · 9.2s)

**Tool: bash**

**Input:**
```json
{
  "command": "cd /tmp/ccache-demo\n\necho \"╔══════════════════════════════════════════════╗\"\necho \"║  1. 首次编译: 缓存未命中 (Miss)              ║\"\necho \"╚══════════════════════════════════════════════╝\"\ntime (ccache g++ -std=c++17 -c main.cpp -o main.o)\nccache -s | grep -E \"Hits:|Misses:\" | head -4\necho\n\necho \"╔══════════════════════════════════════════════╗\"\necho \"║  2. 相同代码: 缓存命中 (Hit - Direct模式)    ║\"\necho \"╚══════════════════════════════════════════════╝\"\ntime (ccache g++ -std=c++17 -c main.cpp -o main.o)\nccache -s | grep -E \"Hits:|Misses:|Direct:|Preprocessed\" | head -5\necho\n\necho \"╔══════════════════════════════════════════════╗\"\necho \"║  3. 源文件内容改变 → Miss                    ║\"\necho \"╚══════════════════════════════════════════════╝\"\nsed -i '' 's/x = 10/x = 20/' main.cpp\nccache g++ -std=c++17 -c main.cpp -o main.o\nccache -s | grep -E \"Hits:|Misses:\" | head -4\nsed -i '' 's/x = 20/x = 10/' main.cpp\necho\n\necho \"╔══════════════════════════════════════════════╗\"\necho \"║  4. 头文件内容改变 → Miss                    ║\"\necho \"╚══════════════════════════════════════════════╝\"\nsed -i '' 's/(n <= 1)/(n < 1)/' math_util.h\nccache g++ -std=c++17 -c main.cpp -o main.o\nccache -s | grep -E \"Hits:|Misses:\" | head -4\nsed -i '' 's/(n < 1)/(n <= 1)/' math_util.h\necho\n\necho \"╔══════════════════════════════════════════════╗\"\necho \"║  5. 编译器选项改变 → Miss                    ║\"\necho \"╚══════════════════════════════════════════════╝\"\nccache g++ -std=c++17 -O2 -c main.cpp -o main.o\nccache -s | grep -E \"Hits:|Misses:\" | head -4\necho\n\necho \"╔══════════════════════════════════════════════╗\"\necho \"║  6. 相同选项再次编译 → Hit                   ║\"\necho \"╚══════════════════════════════════════════════╝\"\nccache g++ -std=c++17 -O2 -c main.cpp -o main.o\nccache -s | grep -E \"Hits:|Misses:|Direct:|Preprocessed\" | head -5\necho\n\necho \"╔══════════════════════════════════════════════╗\"\necho \"║  7. 查看缓存大小                             ║\"\necho \"╚══════════════════════════════════════════════╝\"\nccache -s | grep -E \"Cache size|cache\""
}
```

**Output:**
```
╔══════════════════════════════════════════════╗
║  1. 首次编译: 缓存未命中 (Miss)              ║
╚══════════════════════════════════════════════╝
( ccache g++ -std=c++17 -c main.cpp -o main.o; )  0.25s user 0.06s system 97% cpu 0.319 total
  Hits:               0 /   1 ( 0.00%)
  Misses:             1 /   1 (100.0%)
  Hits:               0 /   1 ( 0.00%)
  Misses:             1 /   1 (100.0%)

╔══════════════════════════════════════════════╗
║  2. 相同代码: 缓存命中 (Hit - Direct模式)    ║
╚══════════════════════════════════════════════╝
( ccache g++ -std=c++17 -c main.cpp -o main.o; )  0.00s user 0.00s system 86% cpu 0.009 total
  Hits:               1 /   2 (50.00%)
    Direct:           1 /   1 (100.0%)
    Preprocessed:     0 /   1 ( 0.00%)
  Misses:             1 /   2 (50.00%)
  Hits:               1 /   2 (50.00%)

╔══════════════════════════════════════════════╗
║  3. 源文件内容改变 → Miss                    ║
╚══════════════════════════════════════════════╝
  Hits:               1 /   3 (33.33%)
  Misses:             2 /   3 (66.67%)
  Hits:               1 /   3 (33.33%)
  Misses:             2 /   3 (66.67%)

╔══════════════════════════════════════════════╗
║  4. 头文件内容改变 → Miss                    ║
╚══════════════════════════════════════════════╝
  Hits:               1 /   4 (25.00%)
  Misses:             3 /   4 (75.00%)
  Hits:               1 /   4 (25.00%)
  Misses:             3 /   4 (75.00%)

╔══════════════════════════════════════════════╗
║  5. 编译器选项改变 → Miss                    ║
╚══════════════════════════════════════════════╝
  Hits:               1 /   5 (20.00%)
  Misses:             4 /   5 (80.00%)
  Hits:               1 /   5 (20.00%)
  Misses:             4 /   5 (80.00%)

╔══════════════════════════════════════════════╗
║  6. 相同选项再次编译 → Hit                   ║
╚══════════════════════════════════════════════╝
  Hits:               2 /   6 (33.33%)
    Direct:           2 /   2 (100.0%)
    Preprocessed:     0 /   2 ( 0.00%)
  Misses:             4 /   6 (66.67%)
  Hits:               2 /   6 (33.33%)

╔══════════════════════════════════════════════╗
║  7. 查看缓存大小                             ║
╚══════════════════════════════════════════════╝
  Cache size (GiB): 0.0 / 5.0 ( 0.01%)

```

---

## Assistant (Build · DeepSeek V4 Flash Free · 3.7s)

_Thinking:_

Now let me also demonstrate the internal hash mechanism and debug mode. Let me show what ccache hashes internally.

Now let's look at what ccache hashes internally using debug mode:

**Tool: bash**

**Input:**
```json
{
  "command": "cd /tmp/ccache-demo && rm -rf main.o && CCACHE_DEBUG=true ccache g++ -std=c++17 -c main.cpp -o main.o && ls -la *.ccache-* 2>/dev/null"
}
```

**Output:**
```
-rw-r--r--@ 1 caviar  wheel     109 Jul 15 22:45 main.o.20260715_224516_655172.ccache-input-c
-rw-r--r--@ 1 caviar  wheel     205 Jul 15 22:45 main.o.20260715_224516_655172.ccache-input-d
-rw-r--r--@ 1 caviar  wheel     376 Jul 15 22:45 main.o.20260715_224516_655172.ccache-input-text
-rw-r--r--@ 1 caviar  wheel  123314 Jul 15 22:45 main.o.20260715_224516_655172.ccache-log

```

---

## Assistant (Build · DeepSeek V4 Flash Free · 3.6s)

**Tool: read**

**Input:**
```json
{
  "filePath": "/tmp/ccache-demo/main.o.20260715_224516_655172.ccache-input-d"
}
```

**Error:**
```
Cannot read binary file: /tmp/ccache-demo/main.o.20260715_224516_655172.ccache-input-d
```

**Tool: read**

**Input:**
```json
{
  "filePath": "/tmp/ccache-demo/main.o.20260715_224516_655172.ccache-input-text"
}
```

**Output:**
```
<path>/tmp/ccache-demo/main.o.20260715_224516_655172.ccache-input-text</path>
<type>file</type>
<content>
1: === COMMON ===
2: 4
3: ### ext
4: ii
5: ### cc_mtime
6: 118928
7: 1775463057000000000
8: ### cc_name
9: g++
10: ### LANG
11: C.UTF-8
12: ### LC_CTYPE
13: UTF-8
14: === DIRECT MODE ===
15: ### cache entry version
16: 1
17: ### result version
18: 0
19: ### manifest version
20: 1
21: ### arg
22: -std=c++17
23: ### inputfile
24: main.cpp
25: ### sourcecode hash
26: a951n1m6q9vbebo3meqai9k76p8sjva22
27: ### sourcecode hash (base16)
28: a951b86c6d27eb72f03b3b4a926873651c9fd421

(End of file - total 28 lines)
</content>
```

**Tool: read**

**Input:**
```json
{
  "filePath": "/tmp/ccache-demo/main.o.20260715_224516_655172.ccache-input-c"
}
```

**Error:**
```
Cannot read binary file: /tmp/ccache-demo/main.o.20260715_224516_655172.ccache-input-c
```

---

## Assistant (Build · DeepSeek V4 Flash Free · 3.6s)

_Thinking:_

Now let me look at the log file to see the detailed flow. Let me also demonstrate what the hash includes.

**Tool: bash**

**Input:**
```json
{
  "command": "grep -i \"hash\\|direct\\|preprocess\\|manifest\\|HIT\\|MISS\\|source\\|include\" /tmp/ccache-demo/main.o.20260715_224516_655172.ccache-log | head -40"
}
```

**Output:**
```
[2026-07-15T22:45:16.655225 38993] Directory configuration file: 
[2026-07-15T22:45:16.655225 38993] Config: (default) direct_mode = true
[2026-07-15T22:45:16.655225 38993] Config: (default) extra_files_to_hash = 
[2026-07-15T22:45:16.655225 38993] Config: (default) hash_dir = true
[2026-07-15T22:45:16.655225 38993] Config: (default) ignore_headers_in_manifest = 
[2026-07-15T22:45:16.655225 38993] Config: (default) read_only_direct = false
[2026-07-15T22:45:16.655225 38993] Working directory: /private/tmp/ccache-demo
[2026-07-15T22:45:16.655225 38993] Apparent working directory: /tmp/ccache-demo
[2026-07-15T22:45:16.655388 38993] Source file: main.cpp
[2026-07-15T22:45:16.655621 38993] Trying direct lookup
[2026-07-15T22:45:16.655691 38993] Inode cache miss: main.cpp
[2026-07-15T22:45:16.655711 38993] Manifest key: 977e4cdb31e1087d83de6feb58dc1c9b521d970a
[2026-07-15T22:45:16.655962 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/print
[2026-07-15T22:45:16.655971 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__vector/pmr.h
[2026-07-15T22:45:16.655986 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__vector/vector_bool.h
[2026-07-15T22:45:16.655993 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__vector/container_traits.h
[2026-07-15T22:45:16.656001 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__algorithm/ranges_copy_n.h
[2026-07-15T22:45:16.656015 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__vector/vector.h
[2026-07-15T22:45:16.656022 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__iterator/ranges_iterator_traits.h
[2026-07-15T22:45:16.656036 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__memory/unique_ptr.h
[2026-07-15T22:45:16.656043 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__type_traits/strip_signature.h
[2026-07-15T22:45:16.656056 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__functional/function.h
[2026-07-15T22:45:16.656066 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/sys/_types/_uid_t.h
[2026-07-15T22:45:16.656073 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__functional/mem_fun_ref.h
[2026-07-15T22:45:16.656080 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__functional/bind.h
[2026-07-15T22:45:16.656086 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__algorithm/unwrap_range.h
[2026-07-15T22:45:16.656098 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/functional
[2026-07-15T22:45:16.656104 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__type_traits/container_traits.h
[2026-07-15T22:45:16.656117 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__split_buffer
[2026-07-15T22:45:16.656124 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__algorithm/ranges_copy.h
[2026-07-15T22:45:16.656132 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__locale_dir/wbuffer_convert.h
[2026-07-15T22:45:16.656145 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__locale_dir/money.h
[2026-07-15T22:45:16.656152 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__memory_resource/memory_resource.h
[2026-07-15T22:45:16.656158 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__algorithm/any_of.h
[2026-07-15T22:45:16.656165 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__algorithm/next_permutation.h
[2026-07-15T22:45:16.656172 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/_types/_nl_item.h
[2026-07-15T22:45:16.656180 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/sys/_types/_fsfilcnt_t.h
[2026-07-15T22:45:16.656186 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/sys/_types/_fsblkcnt_t.h
[2026-07-15T22:45:16.656192 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__functional/pointer_to_unary_function.h
[2026-07-15T22:45:16.656199 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/sys/_types/_fd_zero.h

```

---

## Assistant (Build · DeepSeek V4 Flash Free · 3.9s)

_Thinking:_

Let me get more of the log to show the key decision points - where the cache is looked up, found, etc.

**Tool: bash**

**Input:**
```json
{
  "command": "grep -E \"direct lookup|manifest|MISS|HIT|stored|cache miss|results|using cached|using preprocessor\" /tmp/ccache-demo/main.o.20260715_224516_655172.ccache-log"
}
```

**Output:**
```
[2026-07-15T22:45:16.655225 38993] Config: (default) ignore_headers_in_manifest = 
[2026-07-15T22:45:16.655621 38993] Trying direct lookup
[2026-07-15T22:45:16.655691 38993] Inode cache miss: main.cpp
[2026-07-15T22:45:16.659461 38993] Inode cache miss: ./math_util.h
[2026-07-15T22:45:16.660831 38993] Result entry 0 matched in manifest
[2026-07-15T22:45:16.660874 38993] Got result key from manifest

```

---

## Assistant (Build · DeepSeek V4 Flash Free · 4.7s)

_Thinking:_

Interesting! The direct lookup found a result key from the manifest. That means it was a cache hit. But wait, this was the first compilation after zeroing stats. Let me think...

Actually, looking at the log more carefully: "Result entry 0 matched in manifest" and "Got result key from manifest" - this actually means it found a matching entry in the manifest and got the result key. But the result key might point to a non-existent result file.

Let me check the full log to see whether it was actually a hit or miss.

Actually, looking at the debug text file (ccache-input-text), it shows the components of the hash. Let me show the user the key pieces.

Let me now put together a comprehensive explanation.

**Tool: bash**

**Input:**
```json
{
  "command": "grep -E \"result|storage|manifest|object|DISABLE|using\" /tmp/ccache-demo/main.o.20260715_224516_655172.ccache-log"
}
```

**Output:**
```
[2026-07-15T22:45:16.655225 38993] Config: (default) ignore_headers_in_manifest = 
[2026-07-15T22:45:16.655225 38993] Config: (default) remote_storage = 
[2026-07-15T22:45:16.655747 38993] Retrieved 977e4cdb31e1087d83de6feb58dc1c9b521d970a from local storage (/Users/caviar/Library/Caches/ccache/9/7/7e4cdb31e1087d83de6feb58dc1c9b521d970a)
[2026-07-15T22:45:16.655938 38993] Considering result entry 1 (ba9a95e273d213681a390df7637903e12ce14db2)
[2026-07-15T22:45:16.656412 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__charconv/to_chars_result.h
[2026-07-15T22:45:16.656514 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__memory/raw_storage_iterator.h
[2026-07-15T22:45:16.656563 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__algorithm/in_out_result.h
[2026-07-15T22:45:16.657014 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__type_traits/aligned_storage.h
[2026-07-15T22:45:16.658325 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__type_traits/has_unique_object_representation.h
[2026-07-15T22:45:16.658960 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__functional/weak_result_type.h
[2026-07-15T22:45:16.659371 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__compare/compare_three_way_result.h
[2026-07-15T22:45:16.659420 38993] Considering result entry 0 (1bc94358a1725861bb222a903b9fdf6287ad6f12)
[2026-07-15T22:45:16.659907 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__type_traits/is_object.h
[2026-07-15T22:45:16.660783 38993] Inode cache hit: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__type_traits/result_of.h
[2026-07-15T22:45:16.660831 38993] Result entry 0 matched in manifest
[2026-07-15T22:45:16.660874 38993] Got result key from manifest
[2026-07-15T22:45:16.660906 38993] Retrieved 1bc94358a1725861bb222a903b9fdf6287ad6f12 from local storage (/Users/caviar/Library/Caches/ccache/1/b/c94358a1725861bb222a903b9fdf6287ad6f12)
[2026-07-15T22:45:16.661075 38993] Succeeded getting cached result
[2026-07-15T22:45:16.661082 38993] Result: local_storage_hit
[2026-07-15T22:45:16.661083 38993] Result: local_storage_read_hit
[2026-07-15T22:45:16.661083 38993] Result: local_storage_read_hit

```

---

## Assistant (Build · DeepSeek V4 Flash Free · 198.5s)

_Thinking:_

So even on the first compilation (after zeroing stats), it found a cached result from before! That makes sense - I ran compilations before zeroing stats, and zeroing only resets the counters, not the cached files. 

So the result shows:
1. It read the manifest from local storage
2. It found a matching result entry
3. It retrieved the cached result

This was actually a cache hit from a previous compilation, not a miss. But since I zeroed the stats, the counters were reset but the cache still had data.

Let me redo this with a fresh cache directory to get a true demonstration.

Actually, let me just explain the concepts clearly and use a separate cache dir.

**Tool: bash**

**Input:**
```json
{
  "command": "rm -rf /tmp/ccache-demo /tmp/ccache-test\nmkdir -p /tmp/ccache-test\n\n# main.cpp\ncat > /tmp/ccache-test/main.cpp << 'CPPEOF'\n#include <iostream>\n#include \"math_util.h\"\nint main() {\n    int x = 10;\n    std::cout << \"fact(\" << x << \") = \" << factorial(x) << \"\\n\";\n}\nCPPEOF\n\n# math_util.h\ncat > /tmp/ccache-test/math_util.h << 'HEOF'\n#ifndef MATH_UTIL_H\n#define MATH_UTIL_H\nconstexpr int factorial(int n) { return (n <= 1) ? 1 : n * factorial(n - 1); }\n#endif\nHEOF\n\necho \"=== Files created ===\""
}
```

**Output:**
```
=== Files created ===

```

---

## Assistant (Build · DeepSeek V4 Flash Free · 60.7s)

**Tool: bash**

**Input:**
```json
{
  "command": "cd /tmp/ccache-test && \\\necho \"Step | Description | time\" && \\\necho \"-----|-------------|------\" && \\\necho \"1: 首次编译 (Miss)\" && \\\nCCACHE_DIR=/tmp/ccache-demo-cache CCACHE_NOHASHDIR=1 ccache -z && \\\ntime (CCACHE_DIR=/tmp/ccache-demo-cache CCACHE_NOHASHDIR=1 ccache g++ -std=c++17 -c main.cpp -o main.o 2>&1) 2>&1 | tail -1 | awk '{printf \"    time: %.3fs\\n\", $NF}' && \\\necho \"2: 相同代码 (Hit)\" && \\\ntime (CCACHE_DIR=/tmp/ccache-demo-cache CCACHE_NOHASHDIR=1 ccache g++ -std=c++17 -c main.cpp -o main.o 2>&1) 2>&1 | tail -1 | awk '{printf \"    time: %.3fs\\n\", $NF}' && \\\necho \"3: 修改源文件 (Miss)\" && \\\nsed -i '' 's/x = 10/x = 20/' main.cpp && \\\ntime (CCACHE_DIR=/tmp/ccache-demo-cache CCACHE_NOHASHDIR=1 ccache g++ -std=c++17 -c main.cpp -o main.o 2>&1) 2>&1 | tail -1 | awk '{printf \"    time: %.3fs\\n\", $NF}' && \\\nsed -i '' 's/x = 20/x = 10/' main.cpp && \\\necho \"4: 修改头文件 (Miss)\" && \\\nsed -i '' 's/(n <= 1)/(n < 1)/' math_util.h && \\\ntime (CCACHE_DIR=/tmp/ccache-demo-cache CCACHE_NOHASHDIR=1 ccache g++ -std=c++17 -c main.cpp -o main.o 2>&1) 2>&1 | tail -1 | awk '{printf \"    time: %.3fs\\n\", $NF}' && \\\nsed -i '' 's/(n < 1)/(n <= 1)/' math_util.h && \\\necho \"5: 修改编译选项 (Miss)\" && \\\ntime (CCACHE_DIR=/tmp/ccache-demo-cache CCACHE_NOHASHDIR=1 ccache g++ -std=c++17 -O2 -c main.cpp -o main.o 2>&1) 2>&1 | tail -1 | awk '{printf \"    time: %.3fs\\n\", $NF}' && \\\necho \"6: 相同选项 (Hit)\" && \\\ntime (CCACHE_DIR=/tmp/ccache-demo-cache CCACHE_NOHASHDIR=1 ccache g++ -std=c++17 -O2 -c main.cpp -o main.o 2>&1) 2>&1 | tail -1 | awk '{printf \"    time: %.3fs\\n\", $NF}'"
}
```

**Output:**
```
Step | Description | time
-----|-------------|------
1: 首次编译 (Miss)
Statistics zeroed
( CCACHE_DIR=/tmp/ccache-demo-cache CCACHE_NOHASHDIR=1 ccache g++ -std=c++17   0.25s user 0.07s system 97% cpu 0.328 total
tail -1  0.00s user 0.00s system 0% cpu 0.328 total
awk '{printf "    time: %.3fs\n", $NF}'  0.00s user 0.00s system 0% cpu 0.327 total
2: 相同代码 (Hit)
( CCACHE_DIR=/tmp/ccache-demo-cache CCACHE_NOHASHDIR=1 ccache g++ -std=c++17   0.00s user 0.00s system 83% cpu 0.010 total
tail -1  0.00s user 0.00s system 13% cpu 0.010 total
awk '{printf "    time: %.3fs\n", $NF}'  0.00s user 0.00s system 12% cpu 0.010 total
3: 修改源文件 (Miss)
( CCACHE_DIR=/tmp/ccache-demo-cache CCACHE_NOHASHDIR=1 ccache g++ -std=c++17   0.24s user 0.05s system 96% cpu 0.302 total
tail -1  0.00s user 0.00s system 0% cpu 0.302 total
awk '{printf "    time: %.3fs\n", $NF}'  0.00s user 0.00s system 0% cpu 0.302 total
4: 修改头文件 (Miss)
( CCACHE_DIR=/tmp/ccache-demo-cache CCACHE_NOHASHDIR=1 ccache g++ -std=c++17   0.25s user 0.06s system 97% cpu 0.311 total
tail -1  0.00s user 0.00s system 0% cpu 0.311 total
awk '{printf "    time: %.3fs\n", $NF}'  0.00s user 0.00s system 0% cpu 0.311 total
5: 修改编译选项 (Miss)
( CCACHE_DIR=/tmp/ccache-demo-cache CCACHE_NOHASHDIR=1 ccache g++ -std=c++17   0.25s user 0.05s system 97% cpu 0.310 total
tail -1  0.00s user 0.00s system 0% cpu 0.310 total
awk '{printf "    time: %.3fs\n", $NF}'  0.00s user 0.00s system 0% cpu 0.309 total
6: 相同选项 (Hit)
( CCACHE_DIR=/tmp/ccache-demo-cache CCACHE_NOHASHDIR=1 ccache g++ -std=c++17   0.00s user 0.01s system 84% cpu 0.011 total
tail -1  0.00s user 0.00s system 12% cpu 0.010 total
awk '{printf "    time: %.3fs\n", $NF}'  0.00s user 0.00s system 11% cpu 0.010 total

```

---

## Assistant (Build · DeepSeek V4 Flash Free · 4.3s)

_Thinking:_

Now I have clear results:

1. First compile (miss): 0.328s - actually compiles
2. Same code (hit): 0.010s - 33x faster, just copies from cache
3. Source changed (miss): 0.302s - recompiles
4. Header changed (miss): 0.311s - recompiles
5. Compiler flags changed (miss): 0.310s - recompiles  
6. Same flags (hit): 0.011s - 28x faster

Now let me check the stats to show the hit/miss breakdown.

**Tool: bash**

**Input:**
```json
{
  "command": "CCACHE_DIR=/tmp/ccache-demo-cache ccache -s"
}
```

**Output:**
```
Cacheable calls:      6 /   6 (100.0%)
  Hits:               2 /   6 (33.33%)
    Direct:           2 /   2 (100.0%)
    Preprocessed:     0 /   2 ( 0.00%)
  Misses:             4 /   6 (66.67%)
Local storage:
  Cache size (GiB): 0.0 / 5.0 ( 0.00%)
  Hits:               2 /   6 (33.33%)
  Misses:             4 /   6 (66.67%)

```

---

## Assistant (Build · DeepSeek V4 Flash Free · 13.3s)

_Thinking:_

Now let me clean up the temp files and provide a clear answer. Let me also check the cache directory structure to show what's stored.

**Tool: bash**

**Input:**
```json
{
  "command": "echo \"缓存目录内容:\" && ls /tmp/ccache-demo-cache/ | head -5 && echo \"...\" && find /tmp/ccache-demo-cache -type f | wc -l && echo \"个缓存文件\" && echo \"---\" && echo \"清理临时文件...\" && rm -rf /tmp/ccache-demo-cache /tmp/ccache-test /tmp/ccache-demo"
}
```

**Output:**
```
缓存目录内容:
0
1
2
3
4
...
     287
个缓存文件
---
清理临时文件...

```

---

## Assistant (Build · DeepSeek V4 Flash Free · 7.0s)

## ccache 原理

ccache 的核心思路：**对编译过程的所有输入做哈希，相同哈希结果直接复用，避免重复编译**。

### 三步工作流

```
编译请求 → 计算哈希（输入特征） → 查缓存
                                   ├─ 命中: 直接返回缓存的目标文件（ms级）
                                   └─ 未命中: 执行真实编译 → 存入缓存 → 返回
```

### 哈希包含的内容

哈希输入 = f(源代码内容, 所有头文件内容, 编译器选项, 编译器二进制特征, CWD, 环境变量...)

任何一项变化 → 哈希改变 → cache miss

### 三种查找模式

| 模式 | 工作原理 | 速度 |
|------|----------|------|
| **Direct**（默认） | 哈希源文件+头文件内容，查清单(manifest) | 最快 |
| **Preprocessor**（回退） | 运行预处理器，哈希预处理输出 | 较慢 |
| **Depend** | 利用编译器 `-MD` 输出跟踪头文件依赖 | 类似 Direct |

### 验证结果

```
步骤                | 耗时      | 说明
首次编译 (Miss)     | 0.328s   | 实际编译，结果入缓存
相同代码 (Hit)      | 0.010s   | 33x 加速，Direct模式直接命中
修改源文件 (Miss)   | 0.302s   | 源代码内容变化 → 哈希变
修改头文件 (Miss)   | 0.311s   | 头文件内容变化 → 哈希变
修改编译选项 (Miss) | 0.310s   | -O2 参数变化 → 哈希变
相同选项 (Hit)      | 0.011s   | 28x 加速，再次命中
```

Debug模式下可以看到哈希的具体构成（`ccache-input-text`）：

```
=== DIRECT MODE ===
arg: -std=c++17
inputfile: main.cpp
sourcecode hash: a951b86c6d27eb72f03b3b4a926873651c9fd421
```

ccache 用 **BLAKE3** 计算哈希，用 **XXH3** 校验缓存数据完整性，支持 Zstandard 压缩存储。

---

## User

将生成工程放到当前目录下

---

## Assistant (Build · DeepSeek V4 Flash Free · 3.6s)

_Thinking:_

The user wants me to put the demo project in the current working directory instead of /tmp. The current working directory is `/Volumes/ExtDisk/code/cpp/project/opencodes/ccache`. Let me create the demo files there.

**Tool: bash**

**Input:**
```json
{
  "command": "ls /Volumes/ExtDisk/code/cpp/project/opencodes/ccache"
}
```

**Output:**
```
(no output)
```

---

## Assistant (Build · DeepSeek V4 Flash Free · 10.7s)

_Thinking:_

The directory is empty. Let me create the demo project files there.

**Tool: write**

**Input:**
```json
{
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/ccache/main.cpp",
  "content": "#include <iostream>\n#include \"math_util.h\"\nint main() {\n    int x = 10;\n    std::cout << \"fact(\" << x << \") = \" << factorial(x) << \"\\n\";\n}\n"
}
```

**Output:**
```
Wrote file successfully.
```

**Tool: write**

**Input:**
```json
{
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/ccache/math_util.h",
  "content": "#ifndef MATH_UTIL_H\n#define MATH_UTIL_H\nconstexpr int factorial(int n) { return (n <= 1) ? 1 : n * factorial(n - 1); }\n#endif\n"
}
```

**Output:**
```
Wrote file successfully.
```

**Tool: write**

**Input:**
```json
{
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/ccache/verify.sh",
  "content": "#!/bin/bash\nset -e\n\nCACHE_DIR=$(pwd)/.ccache-cache\nrm -rf \"$CACHE_DIR\" main.o\n\necho \"╔══════════════════════════════════════════════════════╗\"\necho \"║          ccache 原理验证                             ║\"\necho \"╚══════════════════════════════════════════════════════╝\"\necho \"\"\n\n# ---------- 1 ----------\nCCACHE_DIR=$CACHE_DIR CCACHE_NOHASHDIR=1 ccache -z > /dev/null 2>&1\necho \"1. 首次编译 (cache miss)\"\nT1=$({ time CCACHE_DIR=$CACHE_DIR CCACHE_NOHASHDIR=1 \\\n        ccache g++ -std=c++17 -c main.cpp -o main.o 2>&3; } 3>&2 2>&1)\necho \"   耗时: $(echo \"$T1\" | grep real | awk '{print $2}')\"\n\n# ---------- 2 ----------\necho \"2. 相同代码 (cache hit)\"\nT2=$({ time CCACHE_DIR=$CACHE_DIR CCACHE_NOHASHDIR=1 \\\n        ccache g++ -std=c++17 -c main.cpp -o main.o 2>&3; } 3>&2 2>&1)\necho \"   耗时: $(echo \"$T2\" | grep real | awk '{print $2}')\"\n\n# ---------- 3 ----------\nsed -i '' 's/x = 10/x = 20/' main.cpp\necho \"3. 修改源文件 (cache miss)\"\nT3=$({ time CCACHE_DIR=$CACHE_DIR CCACHE_NOHASHDIR=1 \\\n        ccache g++ -std=c++17 -c main.cpp -o main.o 2>&3; } 3>&2 2>&1)\necho \"   耗时: $(echo \"$T3\" | grep real | awk '{print $2}')\"\nsed -i '' 's/x = 20/x = 10/' main.cpp\n\n# ---------- 4 ----------\nsed -i '' 's/(n <= 1)/(n < 1)/' math_util.h\necho \"4. 修改头文件 (cache miss)\"\nT4=$({ time CCACHE_DIR=$CACHE_DIR CCACHE_NOHASHDIR=1 \\\n        ccache g++ -std=c++17 -c main.cpp -o main.o 2>&3; } 3>&2 2>&1)\necho \"   耗时: $(echo \"$T4\" | grep real | awk '{print $2}')\"\nsed -i '' 's/(n < 1)/(n <= 1)/' math_util.h\n\n# ---------- 5 ----------\necho \"5. 修改编译选项 -O2 (cache miss)\"\nT5=$({ time CCACHE_DIR=$CACHE_DIR CCACHE_NOHASHDIR=1 \\\n        ccache g++ -std=c++17 -O2 -c main.cpp -o main.o 2>&3; } 3>&2 2>&1)\necho \"   耗时: $(echo \"$T5\" | grep real | awk '{print $2}')\"\n\n# ---------- 6 ----------\necho \"6. 相同 -O2 选项 (cache hit)\"\nT6=$({ time CCACHE_DIR=$CACHE_DIR CCACHE_NOHASHDIR=1 \\\n        ccache g++ -std=c++17 -O2 -c main.cpp -o main.o 2>&3; } 3>&2 2>&1)\necho \"   耗时: $(echo \"$T6\" | grep real | awk '{print $2}')\"\n\necho \"\"\necho \"╔══════════════════════════════════════════════════════╗\"\necho \"║  统计信息                                           ║\"\necho \"╚══════════════════════════════════════════════════════╝\"\nCCACHE_DIR=$CACHE_DIR ccache -s | grep -E \"Hits:|Misses:|Direct:|Preprocessed:|Cache size|Cacheable\"\n\nrm -rf \"$CACHE_DIR\" main.o\necho \"\"\necho \"清理完成\"\n"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · DeepSeek V4 Flash Free · 4.1s)

**Tool: bash**

**Input:**
```json
{
  "command": "chmod +x /Volumes/ExtDisk/code/cpp/project/opencodes/ccache/verify.sh && bash /Volumes/ExtDisk/code/cpp/project/opencodes/ccache/verify.sh"
}
```

**Output:**
```
╔══════════════════════════════════════════════════════╗
║          ccache 原理验证                             ║
╚══════════════════════════════════════════════════════╝

1. 首次编译 (cache miss)
   耗时: 0m0.330s
2. 相同代码 (cache hit)
   耗时: 0m0.011s
3. 修改源文件 (cache miss)
   耗时: 0m0.311s
4. 修改头文件 (cache miss)
   耗时: 0m0.309s
5. 修改编译选项 -O2 (cache miss)
   耗时: 0m0.318s
6. 相同 -O2 选项 (cache hit)
   耗时: 0m0.010s

╔══════════════════════════════════════════════════════╗
║  统计信息                                           ║
╚══════════════════════════════════════════════════════╝
Cacheable calls:      6 /   6 (100.0%)
  Hits:               2 /   6 (33.33%)
    Direct:           2 /   2 (100.0%)
    Preprocessed:     0 /   2 ( 0.00%)
  Misses:             4 /   6 (66.67%)
  Cache size (GiB): 0.0 / 5.0 ( 0.00%)
  Hits:               2 /   6 (33.33%)
  Misses:             4 /   6 (66.67%)

清理完成

```

---

## Assistant (Build · DeepSeek V4 Flash Free · 4.2s)

_Thinking:_

The demo is complete and working. Let me provide a summary.

已生成到当前目录，文件结构：

```
ccache/
├── main.cpp        # 示例源码
├── math_util.h     # 示例头文件
└── verify.sh       # 验证脚本（不保留缓存，运行后自动清理）
```

运行 `bash verify.sh` 即可看到结果 —— cache hit 约 **0.010s**（30x+ 加速），cache miss 约 **0.31s**（真实编译）。

对比：

| 操作 | 耗时 | 原因 |
|------|------|------|
| 首次编译 | 0.330s | 实际编译 |
| 相同代码 | **0.011s** | Direct 命中 |
| 修改源文件 | 0.311s | 源码哈希改变 |
| 修改头文件 | 0.309s | 头文件哈希改变 |
| 修改编译选项 | 0.318s | 选项加入哈希 |
| 相同选项 | **0.010s** | 再次命中 |

---

