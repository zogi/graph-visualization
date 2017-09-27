Graph visualization demo
========================

## Overview

This demo detects cycles in a graph and removes nodes until it becomes a DAG.
The process is visualized in 3D using [force-based graph visualization](
https://en.wikipedia.org/wiki/Force-directed_graph_drawing).

The graph has 2 types of nodes: resource (green) and process (blue) nodes.
An edge may only exist between nodes of different types, i.e. no
resource to resource or process to process edges may exist.
A resource to process edge means the process has acquired the resource,
while a process to resource edge means the process wants to acquire the
resource.
The demo removes process nodes step-by-step until the graph becomes acyclic,
then it prints a topological ordering of the process nodes which define
a viable running order of the remaining processes.

The demo is written in C++ using OpenGL and GLUT (I know, see the disclaimer).
Only fixed-function pipeline features are used.

## Screenshot

![Screenshot of the graph visualization demo](
http://zogi.github.io/images/screenshot_graph.png)

## Usage

`graph [input_file]`

The `input_file` describes the graph to show. If omitted `input.txt` is used.
The input format is described below.

The first line contains a single positive integer `N`, which is followed
by `N` lines.
Each of the following `N` lines contain a node identifier `id`, a number `M`
denoting the outdegree of the node and `K` node identifiers denoting the
tails of the `K` edges originating from the node `id`.
The node identifiers are strings containing no whitespaces.
The first character of a node identifier defines its type.
Identifiers starting with the character `'f'` (first letter of the word process
in hungarian) define process nodes, other identifiers define resource nodes.
The types of the identifiers following `K` have to be different from the type of
`id`.

### Example input:

    4
    eE 1 fA
    fA 1 eF
    eF 1 fB
    fB 1 eE

## Note to Windows users

Windows binaries of the freeglut library can be downloaded [here](
http://www.transmissionzero.co.uk/software/freeglut-devel/).

The cmake directory contains a FindGLUT.cmake file which (hopefully) will
find glut if the `GLUT_ROOT_PATH` environment variable is set correctly.

For those who don't want to compile the code, a 64 bit Windows binary of this
demo can be downloaded [here](http://zogi.github.io/downloads/graph.zip).

## DISCLAIMER
I've originally written this in 2008 for a university course assignment.
This repository contains deprecated techologies and sophomore-level code
quality that some viewers may find disturbing.
