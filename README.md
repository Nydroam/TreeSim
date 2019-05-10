# TreeSim

# Raymond Wu
# 111133532
# CSE 328

# Specs
Made with Visual Studio 2017 and GLUT

# Instructions

Run Regular.exe for a regular tree
Run Weeping.exe for a weeping tree

Press C to enable/change wind direction
Press W to stop wind direction
Press R to start rotating the tree around the Y axis
Press E to swap the rotation direction
Press S to toggle shading between wireframe and flat

# Advanced Instructions

For further customization of the tree, change the lines from 18 - 54 in main.cpp and rebuild

main_trunk - set true to have a main middle branch for each subtree, set disable to not have this happen.
min_len, max_len - set to minimum allowed length of tree to maximum allowed length of branch starting from the bottom branch (trunk).
min_br, max_br - set to minimum allowed subtree branches at each joint to maximum.
scaleh_min, scaleh_max - set range of how much each branch's height is scaled by its parent branch.
scaleb_min, scaleb_max - set range of how much each branch's radius is scaled by its parent branch.
base_radius, base_height - set height and radius of the bottom branch (trunk).
animframes - set to a higher number for slower animations, lower for faster animations.
