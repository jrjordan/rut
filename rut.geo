{
name: "GEO",
valid_begin: [0, 0],
valid_end: [0, 0],
gdml_file: "rut.gdml",
opdet_lv_name: "volDetector",
}

{
name: "GEO",
index: "border_veto_outer",
valid_begin: [0, 0],
valid_end: [0, 0],
mother:"border",
type:"border",
volume1:"pvTarget",
volume2:"pvDetector0",
surface: "stainless_steel",
reverse: 1,
}

