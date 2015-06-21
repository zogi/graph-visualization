Graph visualization demo
========================

#Overview

This demo detects cycles in a graph and removes nodes until it becomes a DAG.
The process is visualized in 3D using [force-based graph visualization](
https://en.wikipedia.org/wiki/Force-directed_graph_drawing).

The graph has 2 types of nodes: resource (green) and process (blue) nodes.
An edge may only exist between nodes of different types, i.e. no
resource to resource or process to process edges may exist.
A resource to process edge means the process has acquired the resource,
while a process to resource edge means the process wants to acquire the
resource.
The demo kills process nodes step-by-step until the graph becomes acyclic,
then it prints a topological ordering of the process nodes which define
a viable running order of given the remaining processes.

The demo is written in C++ using OpenGL and GLUT (I know, see the disclaimer).
Only fixed-function pipeline features are used.

#Screenshot

![Screenshot of the graph visualization demo](
http://zogi.github.io/images/screenshot_graph.png)

#Usage

`graph [input_file]`

The `input_file` describes the graph to show. If omitted `input.txt` is used.
The input format is described below.

The first line contains a single positive integer `N`, which is followed
by `N` lines. Each line contains a node identifier `id`, a number `M` denoting
the outdegree of the node followed by `K` node identifiers denoting the tails of
the `K` edges originating from `id`.
The node identifiers are strings containing no whitespaces.
The first character of a node identifier defines its type. Identifiers starting
with the character `'f'` (first letter of the word process in hungarian) define
process nodes, other identifiers define resource nodes.
The identifiers following `K` on a single line has to be different from the type
of `id`.

##Example input:

4
eE 1 fA
fA 1 eF
eF 1 fB
fB 1 eE

#DISCLAIMER
I've originally written this in 2008 for a university course assignment.
This repository contains deprecated techologies and sophomore-level code
quality that some viewers may find disturbing.
