
Generic build instructions for OpenCPN plugins on linux.
As example, the "ropeless_pi" plugin build is described

Build the plugin
____________________

1.  Install OpenCPN Version 5.8.4, or later.

2.  Get basic tools required
    $ sudo apt install git cmake

3.  Create the plugin directory:
    $ cd ~

4.  Get the plugin source tree and require submodules from github
    $ git clone https://github.com/bdbcat/ropeless_pi.git
    $ cd ropeless_pi
    $ git checkout -b master
    $ git pull origin master
    $ git submodule update --init opencpn-libs

5.  Get the dependencies and system libraries/tool required to build the plugin
    $ sudo apt install devscripts equivs software-properties-common
    $ mk-build-deps --root-cmd=sudo -ir build-deps/control

6.  Create the build directory
    $ mkdir build
    $ cd build

7A.  Build the plugin (for Ubuntu 22.04)
    $ cmake -DOCPN_TARGET_TUPLE='ubuntu-wx32;22.04;x86_64' ..
    $ make tarball

.or.

7A.  Build the plugin (for Ubuntu 20.04, or earlier)
    $ cmake ..
    $ make tarball

The build product resulting from a successful build process is a tarball file which may be imported into OpenCPN.
The tarball build product is located in the "build" directory, and named something like:
"ropeless-0.4.0.0+2307281851.ab85b9b_ubuntu-wx32-22.04-x86_64.tar.gz"

Import the tarball into OpenCPN
__________________________________
1.  Settings->Plugins->Import Plugin.




