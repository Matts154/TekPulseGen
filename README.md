TekPulseGen
===========

A pulse generator for use with some Tektronix oscilloscopes. It's designed for a specific demo and as such it's not recommended for use in a commercial environment.

It first prompts the user for the number of segments the user wants generated. From there, it prompts the user for segment-specific information; such as the number of pulses and the width. It then displays important information to the user and exports a CSV called "points.csv" in the current working directory.

For the demo purposes, it first generates one pulse with a width of 0.535μs.
