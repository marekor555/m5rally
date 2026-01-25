# Level creation manual

This document outlines basics for creating a m5rally level.


## Level file structure

The levels are saved as a JSON file. This means that levels can be edited without any extra software.

This is an example of a level structure:
```JSON
{
  "car": {
    "x": -40,
    "y": -40,
    "angle": 90
  },
  "boxes": [
    {
      "x": 0,
      "y": 0,
      "w": 25,
      "h": 25,
      "angle": 45
    }
  ],
  "barriers": [
    {
      "points": [
        {"x": -200, "y": 200},
        {"x": 0, "y": 300},
        {"x": 200, "y": 200},
        {"x": 200, "y": -200},
        {"x": -200, "y": -200}
      ]
    }
  ],
  "lines": [
    {
      "points": [
        {"x": -40, "y": 160},
        {"x": -80, "y": 100}
      ]
    }
  ],
  "finishLine": [
    {"x": 40, "y": 160},
    {"x": -40, "y": 160}
  ]
}
```




## How to create more advanced levels

For this step you will need to install Godot game engine (version 4.x recommended). 
Watch some tutorials about this engine, learn how to insert photos, 
move and scale images and how the scene files are structured.

After you have installed it, open a new project, create a new 2D scene and insert a Line2D. 
This line will be your track outline. In its properties you can change its color to red and
widen it, so you will know what it is when you come back after some time.

Then after tracing it, create new Line2D that will be your finish line. 
It can consist of more than 2 points, but I personally like to keep it simple.

(!) Remember do not spam the points, keep the number of points less than 500. Don't torture your cardputer.

Add a Node2D and move it to the position where the car starts, that will be your car's spawn.

After you are done creating the level in the engine, it's time to convert it to the JSON file the game will recognize.
Open the scene file (.tscn) in a text editor and find your objects.

The file should look about like this (the nodes may be in different order):
```TOML
[gd_scene format=3 uid="uid://dui2oy2d7fuwl"]

[node name="Node2D" type="Node2D"] # this is root node, ignore it

[node name="Line2D" type="Line2D" parent="."]
points = PackedVector2Array(<LOTS OF POINTS>) # <- probably your track outline
width = 2.0 
default_color = Color(1, 0, 0, 1)

[node name="Line2D2" type="Line2D" parent="."]
points = PackedVector2Array(<LESS POINTS>) # <- this is probably your finish line
width = 2.0
default_color = Color(0.116666794, 1, 0, 1)

[node name="Node2D" type="Node2D" parent="."]
position = Vector2(88, 489) # <- this is probably your car's position; Vector2(x, y)
```


For car's position copy the x and y values into the JSON and adjust the angle.

For the track outline and finish line, as well as other lines you may make, 
copy the insides of PackedVector2Array and paste it into this python script:

```python
import json

coords_str = input("Enter values: ")

coords = [float(x.strip()) for x in coords_str.split(",")]

if len(coords) % 2 != 0:
    print("Error: please enter an even number of values (x, y pairs).")
    exit(1)

points = [{"x": coords[i], "y": coords[i+1]} for i in range(0, len(coords), 2)]

print("\n\n")
print(json.dumps(points, separators=(",", ":")))

```

Then copy the converted values into "points" for each "barrier" (this is the track outline) or line.
