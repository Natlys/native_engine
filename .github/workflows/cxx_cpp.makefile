name: cxx/cpp

on:
  push:
    branches: [ loc_root ]
  pull_request:
    branches: [ loc_root ]

jobs:
  build:

    runs-on: ubuntu-latest

    steps:
    - uses: actions/checkout@v2
    - name: configure
      run: ./configure
    - name: make
      run: make
    - name: make check
      run: make check
    - name: make distcheck
      run: make distcheck
