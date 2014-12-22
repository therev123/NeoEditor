dofile("Widget.lua")

--- The InputField class
-- The InputField class contains primitives to display and configure
-- UI based edit fields. You can retrieve and change the current text data by
-- using the "setLabel" and "getLabel" functions.
-- Extends the Widget class.

--- InputField(x,y,w,h,text)
-- Creates a new button.
--
-- x: The x position on the canvas
--
-- y: The y position on the canvas
--
-- w: The width of the widget
--
-- h: The height of the widget
--
-- text: The label of the widget
InputField = class(
	Widget,

	function(object, x, y, w, h, text)
		if text == nil then text = "" end
		object.widget = createInput(x,y,w,h,text,"")
		object.position = {x,y}
		object.width = w
		object.height = h
		object.rotation = 0
	end)