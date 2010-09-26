ParentClass = setmetatable({name = "ParentClass"},
{
    __call = function(func, ...)
        return setmetatable({}, {__type = getmetatable(func).__type});
    end,
    __type = function(obj)
        return "ParentClass";
    end
});

MyClass = setmetatable({name = "MyClass", inherits = true},
{
    __call = function(func, ...)
        return setmetatable({inherits = true}, {__type = getmetatable(func).__type});
    end,
    __type = function(obj)
        if (obj == nil) then
            return {"MyClass", ParentClass};
        end
        if (obj.inherits) then
            return {"MyClass", ParentClass};
        else
            return "MyClass"
        end
    end
});

string = setmetatable({name = "string"},
{
    __type = function()
        return "string";
    end
});

number = setmetatable({name = "number"},
{
    __type = function()
        return "number";
    end
});

function RunType()
    -- Check constructor-style
    inst = MyClass()
    if (inst is MyClass) then
        print("inst is an instance of MyClass")
    else
        print("inst is not an instance of MyClass")
    end

    if (inst is ParentClass) then
        print("inst is an instance of ParentClass")
    else
        print("inst is not an instance of ParentClass")
    end
    
    -- Now change a variable in inst...
    inst.inherits = false
    if (inst is ParentClass) then
        print("inst is still an instance of ParentClass")
    else
        print("inst is no longer an instance of ParentClass")
    end
    
    -- Check string / number
    str = "abced"
    if (str is string) then
        print("'abced' is a string")
    end
    num = 4
    if (num is number) then
        print("4 is a number")
    end
end

function Run()
    --[[print("Engine.DivideByZeroException" .. ": <table>")
    InspectTable(Engine.DivideByZeroException, "                            " .. "    ");
    print("                            " .. "  <metatable>")
    InspectTable(getmetatable(Engine.DivideByZeroException), "                            " .. "    ");
    
    print("MyClass" .. ": <table>")
    InspectTable(MyClass, "       " .. "    ");
    print("       " .. "  <metatable>")
    InspectTable(getmetatable(MyClass), "       " .. "    ");
    
    print("string" .. ": <table>")
    InspectTable(string, "      " .. "    ");
    print("      " .. "  <metatable>")
    InspectTable(getmetatable(string), "      " .. "    ");

    av = Engine.AccessViolationException]]
    
    try
        try
            try
                error("abc")
            catch (e is number) do
                print("Caught numeric exception!")
            end
        catch (e is string) do
            print("Caught string exception!")
            error(Engine.Exception())
        end
    catch (e is Engine.Exception) do
        print("Caught Engine.Exception!")
    end

    clr = Engine.Collections.Color(1, 2, 3, 4);
    print(getmetatable(clr).__type(clr))
    print("clr is an " .. tostring(getmetatable(Engine.Collections.Color).__type()))
    if (clr is Engine.Collections.Color) then
        print("clr is an Engine.Collections.Color");
    else
        print("clr is not an Engine.Collections.RColor");
    end
end

function TestCatch()
    error("abc")

    try
        error("abc")
    catch (e is number) do
        print("Caught numeric exception!")
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
