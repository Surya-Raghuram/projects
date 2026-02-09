Python libraries used:
   - BioPython
   - NumPy


Files Included:
//proteins used
1MBN.pdb 
1UBQ.pdb
1CRN.pdb
2PTC.pdb
1A3N.pdb
//python and pymol scripts
successive_distance.py
angles.py
end_distance.py
phi_angles.pml
//outputs
successive_distance.txt
angles.txt
end_distance.txt
phi_angles.txt


Ensure that all PDB files and Python scripts are present
in the same directory.
All outputs will be printed in the terminal

1. To calculate inter-residue (C-alpha) distances between
   successive residues:

   python distance_successive.py

2. To calculate angles formed by three successive C-alpha
   atoms:

   python ca_angles.py

3. To calculate end-to-end distances (distance between
   first and last C-alpha atoms) for multiple proteins:

   python end_to_end.py


Phi (φ) dihedral angles were extracted using PyMOL.

1. Run PyMOL in command-line mode:

   pymol -cq phi_angles.pml


Outputs:

The outputs of all scripts have been attached as text files for reference.
