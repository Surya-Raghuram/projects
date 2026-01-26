import numpy as np
from Bio.PDB import PDBParser
import math

parser = PDBParser(QUIET=True)
structure = parser.get_structure("MBN", "1MBN.pdb")

ca_coords = []

for model in structure:
    for chain in model:
        for residue in chain:
            if "CA" in residue:
                ca_coords.append(residue["CA"].get_coord())

angles = []

for i in range(1, len(ca_coords)-1):
    A = ca_coords[i-1]
    B = ca_coords[i]
    C = ca_coords[i+1]

    BA = A - B
    BC = C - B

    cosang = np.dot(BA, BC) / (np.linalg.norm(BA)*np.linalg.norm(BC))
    angle = math.degrees(math.acos(cosang))

    angles.append(angle)

print("First 10 C-alpha angles:", angles[:10])
print("Mean CA angle:", np.mean(angles))
