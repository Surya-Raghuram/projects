import numpy as np
from Bio.PDB import PDBParser

def end_to_end(pdb_file):
    parser = PDBParser(QUIET=True)
    structure = parser.get_structure("X", pdb_file)

    ca_coords = []
    for model in structure:
        for chain in model:
            for residue in chain:
                if "CA" in residue:
                    ca_coords.append(residue["CA"].get_coord())

    start = ca_coords[0]
    end = ca_coords[-1]

    return np.linalg.norm(end - start)

files = ["1MBN.pdb", "1UBQ.pdb", "1CRN.pdb", "2PTC.pdb", "1A3N.pdb"]

for f in files:
    d = end_to_end(f)
    print(f, "End-to-end distance =", d)
