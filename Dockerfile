FROM ubuntu:18.04

RUN apt-get update && apt-get -y install tar xz-utils rsync build-essential
COPY clang+llvm-15.0.5-x86_64-linux-gnu-ubuntu-18.04.tar.xz /usr/src/
RUN xz -dc /usr/src/clang+llvm-15.0.5-x86_64-linux-gnu-ubuntu-18.04.tar.xz | tar xfv - -C /usr
RUN rsync -a /usr/clang+llvm-15.0.5-x86_64-linux-gnu-ubuntu-18.04/bin/ /usr/local/bin &&\
    rsync -a /usr/clang+llvm-15.0.5-x86_64-linux-gnu-ubuntu-18.04/include/ /usr/local/include &&\
    rsync -a /usr/clang+llvm-15.0.5-x86_64-linux-gnu-ubuntu-18.04/lib/ /usr/local/lib &&\
    rsync -a /usr/clang+llvm-15.0.5-x86_64-linux-gnu-ubuntu-18.04/share/ /usr/local/share &&\
    mv /usr/clang+llvm-15.0.5-x86_64-linux-gnu-ubuntu-18.04/libexec /usr/local/ &&\
    rm -r /usr/clang+llvm-15.0.5-x86_64-linux-gnu-ubuntu-18.04