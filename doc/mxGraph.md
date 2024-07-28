
a mxGraphModel

We use the "id" attribute of the the mxCell nodes in an mxGraphModel.

## nodes
To specify that an mxCell is a node: `id="node:<node name>"` where `"node name"` is a unique name for this node.  This will be used as the node key. Note that you can just use `"n"` instead of `"node"`, i.e. `id="n:<node name>"`.

## inputs and outputs
To specify that an mxCell is an input for a node: `id="input:<node name>:<input name>"` where `"input name"` is a unique name within this node for this input. Note that you can just use `"i"` instead of `"input"`, i.e. `id="i:<node name>:<input name>"`

To specify that an mxCell is an output for a node: `id="output:<node name>:<output name>"` where `"output name"` is a unique name within this node for this output.

To specify that an mxCell is a beam: `id="beam:<beam name>"` where `"beam name"` is a unique name for the beam.  The `"source"` attribute of a beam mxCell must have a value that is an output node id, so `source="output:<node name>:<output name>"`.  Similarly the `"target"` attribute value must be an input node id, so `source="input:<node name>:<input name>"`

## beams

`id="beam:<beam-name>`
`source="output:<node name>:<output name>"`
`target="input:<node name>:<input name>"`

## output links 

`id="link:<link-name>`
note: link-name is ignored, but must be unique for drawio to work.

`source="input:<node name>:<input name>"`
`target="output:<node name>:<output name>"`

note: source and target are swapped w.r.t. input/output comapred to a beam.

