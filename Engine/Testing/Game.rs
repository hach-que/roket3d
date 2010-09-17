function Run()
    ex = Engine.ContextNotProvidedException()
    print("ex:")
    InspectTable(ex, "  ");
    print("ex (metatable):")
    InspectTable(getmetatable(ex), "  ");
    print("Created exception " .. tostring(ex.Message));

    try
        vec = Engine.Collections.RVector3D(0, 0, 0);
        vec:Invert();
    catch e do
        print("e:")
        InspectTable(e, "  ");
        print("e (metatable):")
        InspectTable(getmetatable(e), "  ");
        print("Caught exception " .. tostring(e.Message));
    end
end

function RunOld()
    print("Hello World!");
    print("Engine:")
    InspectTable(Engine, "  ");
    vec = Engine.Collections.RVector3D(12, 22, 33);
    print("vec:")
    InspectTable(vec, "  ");
    print("vec (metatable):")
    InspectTable(getmetatable(vec), "  ");
    print(vec.X);
    print(vec.Y);
    print(vec.Z);
    vec.X = 5
    print(vec.X);
end

function InspectTable(tbl, indent)
    for k, v in pairs(tbl) do
        if (type(v) == "table") then
            print(indent .. k .. ":")
            InspectTable(v, indent .. "  ");
        else
            print(indent .. k .. ": " .. tostring(v))
        end
    end
end
