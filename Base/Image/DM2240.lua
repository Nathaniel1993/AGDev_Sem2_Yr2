function SaveToLuaFile(outputString, overwrite)
   print("SaveToLuaFile...")
   local f;						-- The file
   if overwrite == 1 then		-- Wipe the contents with new data
      f = assert(io.open("Image/DM2240_HighScore.lua", "w"))
   elseif overwrite == 0 then	-- Append with new data
      f = assert(io.open("Image/DM2240_HighScore.lua", "a"))
   end
   -- Write to the file
   f:write(outputString)
   -- Close the file
   f:close()
   print("OK")
end
function CalculateDistanceSquare(x1,y1,z1,x2,y2,z2)
	local distanceSquare = (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1) + (z2 - z1)*(z2 - z1)
	print (distanceSquare)
	return distanceSquare
end
function GetMinMax(...)
local minValue = -1
local maxValue = -1
local avgValue = 0
local numValue = 0

for i,v in ipairs(arg) do
		if minValue == -1 then
			minValue = v
		elseif v < minValue then
			minValue = v
		end

		if maxValue == -1 then
			maxValue = v
		elseif v > maxValue then
			maxValue = v
		end
		
		avgValue = avgValue + v / (numValue + 1)
		numValue = numValue + 1
    end
	avgValue = avgValue / numValue
	print(minValue, maxValue, avgValue, numValue)
	return minValue, maxValue, avgValue, numValue


end
title = "DM2240 - Assignment"
width = 800
height = 600
PlayerPos_x = 100
PlayerPos_z = 200
keyFORWARD = "Z";

-- Keybourd Inputs
moveForward  = "W"
moveBackward = "S"
moveLeft 	 = "A"
moveRight 	 = "D"

-- CPlayerInfoStartPos
CPlayerInfoStartPos = {0,0,10}

-- Waypoints
Waypoint_A_1 = {x = 100.0, y = 0.0, z = 200.0}
Waypoint_A_2 = {x = 150.0, y = 0.0, z = 0.0}
Waypoint_A_3 = {x = 100.0, y = 0.0, z = -200.0}
Waypoint_A_4 = {x = 0.0, y = 0.0, z = -250.0}
Waypoint_A_5 = {x = -100.0, y = 0.0, z = -200.0}
Waypoint_A_6 = {x = -150.0, y = 0.0, z = 0.0}
Waypoint_A_7 = {x = -100.0, y = 0.0, z = 200.0}
Waypoint_A_8 = {x = 0.0, y = 0.0, z = 250.0}