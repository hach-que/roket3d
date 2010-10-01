function Run()
    device = Engine.Unmanaged.Core.Device(
        Engine.Collections.Vector2D(640, 480),
        16, false, true, false);
        
    backcolor = Engine.Collections.Color(255, 255, 0, 0)
    up = true
    
    -- Add a GUI button
    button = device.GUIEnvironment:AddButton(
		Engine.Collections.Rectangle(20, 20, 100, 40),
		nil, nil, "My Button!")
    
    while (device.Running) do
        if (up) then
            backcolor.Green = backcolor.Green + 1
            if (backcolor.Green == 255) then
                up = false
            end
        else
            backcolor.Green = backcolor.Green - 1
            if (backcolor.Green == 0) then
                up = true
            end
        end
        backcolor.Red = 255 - backcolor.Green
        device.VideoDriver:BeginScene(backcolor, true, true);
        device.GUIEnvironment:Draw();
        device.VideoDriver:EndScene();
    end
    
    device.Close()
end

function InspectTable(tbl, indent)
    if (indent == nil) then
        indent = ""
    end
    local printed = false
    for k, v in pairs(tbl) do
        printed = true
        if (type(v) == "table") then
            print(indent .. k .. ": <table>")
            InspectTable(v, indent .. "     ");
            print(indent .. "   <metatable>")
            InspectTable(getmetatable(v), indent .. "     ");
        elseif (type(v) == "number") then
            print(indent .. k .. ": <number> " .. tostring(v))
        elseif (type(v) == "string") then
            print(indent .. k .. ": <string> " .. tostring(v))
        elseif (type(v) == "function") then
            if (k == "__type") then -- only inspect types further
                if (type(v()) == "string") then
                    print(indent .. k .. ": <fstring> " .. v())
                else
                    print(indent .. k .. ": <function>")
                    InspectTable(v(), indent .. "  ");
                end
            end
        else
            print(indent .. k .. ": <" .. type(v) .. ">")
        end
    end
    if (printed == false) then
        print(indent .. "<empty>")
    end
end
