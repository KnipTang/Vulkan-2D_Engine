# 2D Line Renderer

A 2D Line Renderer made in Vulkan using C++. ![ProgramExample](https://github.com/KnipTang/Vulkan-2D_Engine/blob/main/Images/Program.PNG?raw=true)
# Features

The following primitives are supported:
	Rectangle and oval, the oval supports setting
	the amount of vertices used,
	Round rectangle,
	Arc with start and stop angle,
	and a Torus (doughnut shape).
These shapes can be rendered filled or unfilled. The border can be turned on or off and customized In-Real-Time. All colors of the border and fill can be changed as well.
	
Users can easily create algorithms for custom shapes.

A grid is supported to get a better feeling of location while placing shapes. The grid can be turned off and changed In-Real-Time.

An infinite amount of lines can be drawn and erased using the mouse.

All colors and the width of every line can be customized In-Real-Time using keyboard shortcuts.

A depth buffer is included allowing each element's depth to be set to create layers in your digital art pieces.

Controls:
![Controls](https://github.com/KnipTang/Vulkan-2D_Engine/blob/main/Images/Controls.PNG?raw=true)

# Development

This Line Renderer was fully made in C++ using the Vulkan SDK. The line rendering was possible using the **VK_PRIMITIVE_TOPOLOGY_LINE_LIST** topology. 

# Resources

[Vulkan tutorial](https://vulkan-tutorial.com/)
