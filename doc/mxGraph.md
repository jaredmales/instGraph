# mxGraph and drawio

We can use the mxGraph XML format, the format used by drawio, to represent an instGraph. The "id" attribute of the mxCell nodes are used to encode what each item represents.

## nodes

To specify that an mxCell is a node: `id="node:<node name>"` where `"node name"` is a unique name for this node.  This will be used as the node key. Note that you can just use `"n"` instead of `"node"`, i.e. `id="n:<node name>"`.

## inputs and outputs

Each node has inputs and outputs. To specify that an mxCell is an input for a node: `id="input:<node name>:<input name>"` where `"input name"` is a unique name within this node for this input. Note that you can just use `"i"` instead of `"input"`, i.e. `id="i:<node name>:<input name>"`

To specify that an mxCell is an output for a node: `id="output:<node name>:<output name>"` where `"output name"` is a unique name within this node for this output. Note that you can just use `"o"` instead of `"output"`, i.e. `id="o:<node name>:<output name>"`


## beams

A beam, equivalent to an edge, connects outputs to inputs. 

To specify that an mxCell is a beam: `id="beam:<beam name>"` where `"beam name"` is a unique name for the beam.  You can just use `b` instead of `beam`.  The `"source"` attribute of a beam mxCell must have a value that is an output node id, so `source="output:<node name>:<output name>"`.  Similarly the `"target"` attribute value must be an input node id, so `target="input:<node name>:<input name>"`

## output links 

An "output link" is a beam internal to a node, that connects an input to an output.  This is done so an input becoming on results in the linked output becomig on.  They are specified like a beam: `id="link:<link-name>`, and you can just use `l` instead of `link`.

note: link-name is ignored, but must be unique for drawio to work.

Compared to a `beam`, however, source and target are swapped w.r.t. input/output.

`source="input:<node name>:<input name>"`

`target="output:<node name>:<output name>"`


