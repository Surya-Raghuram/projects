import random
import math


# sample sequence of 1PRB from 
seq_1PRB = "MKQLQNVLKNLSDDLKNNNQLLEQLNSDQLAEQKKELENQLKQLAEQLK"


def hp_model_simulation(sequence, hydrophobic, hydrophilic):
    n = len(sequence)

    hp_seq = ['H' if aa in hydrophobic else 'P' for aa in sequence]

    directions = [(1,0,0),(-1,0,0),(0,1,0),(0,-1,0),(0,0,1),(0,0,-1)]

    coords = [(0,0,0)]
    occupied = set(coords)

    # Self avoiding walk
    for i in range(1, n):
        x,y,z = coords[-1]
        random.shuffle(directions)

        placed = False
        for dx,dy,dz in directions:
            new = (x+dx,y+dy,z+dz)
            if new not in occupied:
                coords.append(new)
                occupied.add(new)
                placed = True
                break

        if not placed:
            return (0,0,0,0)

    coord_map = {coords[i]:i for i in range(n)}
    energy = 0

    for i,(x,y,z) in enumerate(coords):
        if hp_seq[i] != 'H':
            continue

        for dx,dy,dz in directions:
            neigh = (x+dx,y+dy,z+dz)
            if neigh in coord_map:
                j = coord_map[neigh]
                if abs(i-j) > 1 and hp_seq[j]=='H':
                    energy -= 1

    energy = energy//2

    cx = sum(c[0] for c in coords)/n
    cy = sum(c[1] for c in coords)/n
    cz = sum(c[2] for c in coords)/n

    h_dist = []
    for i,c in enumerate(coords):
        if hp_seq[i]=='H':
            d = math.sqrt((c[0]-cx)**2+(c[1]-cy)**2+(c[2]-cz)**2)
            h_dist.append(d)

    avg_core = sum(h_dist)/len(h_dist) if h_dist else 0
    max_dist = max(math.sqrt((c[0]-cx)**2+(c[1]-cy)**2+(c[2]-cz)**2) for c in coords)

    return coords, energy, avg_core, max_dist

hydrophobic = ['A','V','I','L','M','F','W','Y']
hydrophilic = ['R','N','D','Q','E','K','S','T','H','C','G','P']

coords, energy, avg_core, max_dist = hp_model_simulation(seq_1PRB, hydrophobic, hydrophilic)

print("Coordinates:", coords)
print("Energy:", energy)
print("Average hydrophobic core distance:", avg_core)
print("Maximum structure radius:", max_dist)
