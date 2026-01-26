load 1MBN.pdb

python
from pymol import cmd

phi_angles = []

# get all residues with CA atoms
residues = cmd.get_model("1MBN and name CA").atom
resi_numbers = [int(a.resi) for a in residues]

for i in range(1, len(resi_numbers)):
    r_prev = resi_numbers[i-1]
    r_curr = resi_numbers[i]

    try:
        phi = cmd.get_dihedral(
            f"1MBN and resi {r_prev} and name C",
            f"1MBN and resi {r_curr} and name N",
            f"1MBN and resi {r_curr} and name CA",
            f"1MBN and resi {r_curr} and name C"
        )
        phi_angles.append(phi)
    except:
        pass

print("First 10 Phi angles:")
print(phi_angles[:10])
print("Total Phi angles:", len(phi_angles))
python end
