from Bio.PDB import PDBParser
import numpy as np

parser = PDBParser(QUIET=True)
structure = parser.get_structure("MBN", "1MBN.pdb")

ca_coords = []

for model in structure:
    for chain in model:
        for residue in chain:
            if "CA" in residue:
                ca_coords.append(residue["CA"].get_coord())

distances = []

for i in range(len(ca_coords)-1):
    d = np.linalg.norm(ca_coords[i+1] - ca_coords[i])
    distances.append(d)

distances = np.array(distances)

print("First 10 distances:", distances[:10])
print("Mean distance:", np.mean(distances))
print("Std deviation:", np.std(distances), "~0. This shows that the distances between successive C-alpha atoms are almost constant.")
