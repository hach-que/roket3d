-- Overwrite our loading functionality.
package.loaders = {}

package.loaders[1] = function(module, env)
	-- Declare our variables local so they don't
	-- interfere with the main program.
	if (env == nil) then
		env = _G
	end
	
	local path, ext, found_path, e, fhandle
	local chunk, lmodule, _T, lfmodulepath
	local indent, l, n, ldotpos, classname
	local namespace, class_funcs, class, mt
	local __init, f, v, k, tdotpos, __base, i
	local class_inheritance = {}
	local inherit_classname, inherit_namespaces
	local inheritance_sandbox, i2
	local function func_inherit(class)
		if (lmodule._T == nil) then
			lmodule._T = {}
		end
		
		table.insert(class_inheritance, class)
	end
	local function func_name(name)
		local ldotpos, tdotpos, classname, namespaces
		local z, x, c
		ldotpos = string.len(name)
		tdotpos = string.len(name)
		classname = nil
		namespaces = {}
		while (ldotpos > 0) do
			if (string.sub(name,ldotpos,ldotpos) == ".") then
				if (classname == nil) then
					classname = string.sub(name, ldotpos + 1, tdotpos)
				else
					table.insert(namespaces,1,string.sub(name, ldotpos + 1, tdotpos))
				end
				tdotpos = ldotpos - 1
			end
			ldotpos = ldotpos - 1
		end
		if (tdotpos > 0) then
			table.insert(namespaces,1,string.sub(name, ldotpos + 1, tdotpos))
		end
		z = {}
		for c, x in ipairs(namespaces) do
			table.insert(z,1,x)
		end
		namespaces = z
		
		if (lmodule._T == nil) then
			lmodule._T = {}
		end
		
		lmodule._T["NAME"] = classname
		if (#namespaces > 0) then
			lmodule._T["NAMESPACE"] = namespaces
		else
			lmodule._T["NAMESPACE"] = nil
		end
	end
	local function func_description(desc)
		if (lmodule._T == nil) then
			lmodule._T = {}
		end
		
		lmodule._T["DESCRIPTION"] = desc
	end
	local function func_author(author)
		if (lmodule._T == nil) then
			lmodule._T = {}
		end
		
		lmodule._T["AUTHOR"] = author
	end
	local function func_lastmodified(date)
		if (lmodule._T == nil) then
			lmodule._T = {}
		end
		
		lmodule._T["LASTMODIFIED"] = date
	end
	
	-- Replace the "." in the requested module with
	-- backslashes.
	path = string.gsub(module, "%.", "/")
	path = "./" .. path
	ext = {"rcs", "rs", "rks", "lua"}
	found_path = nil
	
	for k, e in pairs(ext) do
		fhandle = io.open(path .. "." .. e, "r")
		if (fhandle ~= nil) then
			fhandle:close()
			found_path = path .. "." .. e
			break
		end
	end
	
	if (found_path == nil) then
		print("ERR : Unable to locate module at " .. path .. ".{rcs,rs,rks,lua}.")
		return nil, "Unable to locate module at " .. path .. ".{rcs,rs,rks,lua}."
	end
	
	-- Since the file exists, we're now going to load it.
	chunk = loadfile(found_path)
	if (chunk == nil) then
		print("ERR : Module " .. module .. " contains syntax errors and cannot be included in the program.")
		return nil, "Module " .. module .. " contains syntax errors and cannot be included in the program."
	end
	
	-- Isolate the class name from the namespace component
	ldotpos = string.len(module)
	while (ldotpos > 0) do
		if (string.sub(module,ldotpos,ldotpos) == ".") then
			break
		end
		ldotpos = ldotpos - 1
	end
	classname = string.sub(module, ldotpos + 1)
	if (ldotpos ~= 0) then
		namespace = string.sub(module, 1, ldotpos - 1)
	else
		namespace = ""
	end
	
	-- Run the chunk() function inside a sandbox, so we can inspect the module.
	lmodule = {}
	lmodule[classname] = {}
	lmodule["inherits"] = func_inherit
	lmodule["name"] = func_name
	lmodule["description"] = func_description
	lmodule["author"] = func_author
	lmodule["lastmodified"] = func_lastmodified
	setfenv(chunk, lmodule)
	chunk()

	-- Check to see if _T exists, if it doesn't, show that the module can't be loaded.
	if (lmodule["_T"] == nil) then
		print("ERR : Module " .. module .. " does not specify module information.")
		return nil, "Module " .. module .. " does not specify module information."
	end
	
	-- Move the _T table from the code block, into a local variable.
	_T = lmodule["_T"]
	lmodule["_T"] = nil
	
	-- Sanitize the _T variable we will use.
	_T["NAME"]			= tostring(_T["NAME"])
	if (_T["DESCRIPTION"] ~= nil) then
		_T["DESCRIPTION"]	= tostring(_T["DESCRIPTION"])
	end
	if (_T["AUTHOR"] ~= nil) then
		_T["AUTHOR"]		= tostring(_T["AUTHOR"])
	end
	if (_T["LASTMODIFIED"] ~= nil) then
		_T["LASTMODIFIED"]	= tostring(_T["LASTMODIFIED"])
	end
	
	-- Verify that the module is located at the correct location (NAMESPACE "." NAME == module)
	if (_T["NAMESPACE"] ~= nil) then
		lfmodulepath = table.concat(_T["NAMESPACE"], ".") .. "." .. _T["NAME"]
		if (lfmodulepath ~= module) then
			print("ERR : Module name mismatch.  Loaded from " .. module .. ", but code specifies " .. lfmodulepath .. ".")
			return nil, "Module name mismatch.  Loaded from " .. module .. ", but code specifies " .. lfmodulepath .. "."
		end
	else
		lfmodulepath = _T["NAME"]
	end
	
	-- Now create the namespace tables if required.
	l = env
	if (_T["NAMESPACE"] ~= nil) then
		for k, n in pairs(_T["NAMESPACE"]) do
			l[n] = {}
			l = l[n]
		end
	end
	
	-- Get the class functions.
	class_funcs = lmodule[classname]
	lmodule[classname] = nil
	__init = nil
	__base = nil
	
	-- Build up the class.
	class = {}
	if (#class_inheritance > 0) then
		-- We need to evaluate all of the inherited class
		-- in order, to build up.
		for k, v in pairs(class_inheritance) do
			-- Isolate the class name from the namespace component
			print("INFO: Loading " .. v .. " in required module.  Namespaces / classes should not leak.")
			ldotpos = string.len(v)
			tdotpos = string.len(v)
			inherit_classname = nil
			inherit_namespaces = {}
			while (ldotpos > 0) do
				if (string.sub(v,ldotpos,ldotpos) == ".") then
					if (inherit_classname == nil) then
						inherit_classname = string.sub(v, ldotpos + 1, tdotpos)
					else
						table.insert(inherit_namespaces,1,string.sub(v, ldotpos + 1, tdotpos))
					end
					tdotpos = ldotpos - 1
				end
				ldotpos = ldotpos - 1
			end
			if (tdotpos > 0) then
				table.insert(inherit_namespaces,1,string.sub(v, ldotpos + 1, tdotpos))
			end
			
			inheritance_sandbox = {}
			package.loaders[1](v, inheritance_sandbox)
			
			-- Load the class into i.
			i = inheritance_sandbox
			for i2, n in pairs(inherit_namespaces) do
				i = i[n]
			end
			i = i[inherit_classname]
			
			-- Now copy all of the inherited classes functions.
			for n, f in pairs(i) do
				if (n == "__init") then
					__base = f
					setfenv(__base, env)
				elseif (type(f) == "function") then
					class[n] = f
					setfenv(class[n], env)
				elseif (type(f) ~= "function") then
					class[n] = f
				end
			end
		end
	end
	for n, f in pairs(class_funcs) do
		if (n == "__init") then
			__init = f
			setfenv(__init, env)
		else
			class[n] = f
			setfenv(class[n], env)
		end
	end
	for n, v in pairs(lmodule) do
		if (n ~= "inherits"
			and n ~= "name"
			and n ~= "description"
			and n ~= "author"
			and n ~= "lastmodified") then
			if (type(v) == "function") then
				print("WARN: " .. lfmodulepath .. ":0: Function " .. n .. " defined without class context.  It is not included in the class definition.")
			else
				-- Make the specified variable a static variable.
				class[n] = v
			end
		end
	end
	mt = {}
	mt.__call = function(class_tbl, ...)
		local obj = {}
		setmetatable(obj, class)
		if (__init ~= nil) then
			__init(obj, ...)
		elseif (__base ~= nil) then
			__base(obj, ...)
		end
		return obj
	end
	class.__index = class
	class.__init = __init
	class.__base = __base
	setmetatable(class, mt)
	
	-- Now put the newly generated class in the namespace.
	l[classname] = class
	
	-- Show module loaded message
	print("INFO: " .. lfmodulepath .. ":0: Loaded module " .. _T["NAME"] .. ".")
	indent = string.rep(" ", string.len("INFO: " .. lfmodulepath .. ":0: "))
	if (_T["NAMESPACE"] ~= nil) then
		print(indent .. "    in namespace " .. namespace)
	end
	if (_T["DESCRIPTION"] ~= nil) then
		print(indent .. "    Description: " .. _T["DESCRIPTION"])
	end
	if (_T["AUTHOR"] ~= nil) then
		print(indent .. "    Author: " .. _T["AUTHOR"])
	end
	if (_T["LASTMODIFIED"] ~= nil) then
		print(indent .. "    Last Modified: " .. _T["LASTMODIFIED"])
	end
	
	return function()
	end
end

-- Now test our package loader.

require "AnotherNamespace.ClassB"

print "===== AnotherNamespace.ClassB ====="
for k, v in pairs(AnotherNamespace.ClassB) do
	print(k,v)
end

print "===== Constructor ====="
a = AnotherNamespace.ClassB()

print "===== a ====="
for k, v in pairs(a) do
	print(k,v)
end

print "===== Checking namespaces... ====="
print("MainNamespace", MainNamespace)
print("AnotherNamespace", AnotherNamespace)

print "===== Checking inheritance... ====="
a:Function()
a:AnotherFunction()

-- The loader below doesn't work because you can't issue
-- function calls without brackets if the argument is not
-- a string literal or table.
--[[
package.loaders[1] = setmetatable({

	-- Catch references to Namespace.Class.  After the user has written
	-- out a name in the form of Namespace.Class, this function must be
	-- called again to rehook rehook the __index metamethod.
	StartCapture = function(env)
		-- Add the index metamethod to the environment.
		local f = function(t, key)
			print("Stopped capturing on " .. tostring(env) .. "!")
			getmetatable(env)["__index"] = nil
			
			local s, k, v
			if (t == env) then
				s = { key }
			else
				s = { }
				for k, v in pairs(t["value"]) do
					s[#s + 1] = v
				end
				s[#s + 1] = key
			end
			return setmetatable({ value = s, next = env }, { __index = f });
		end
		print("Started capturing on " .. tostring(env) .. "!")
		if (getmetatable(env) == nil) then
			setmetatable(env, {})
		end
		getmetatable(env)["__index"] = f
	end,
	
	-- Handle name keyword
	Class = function(t)
		if (type(t) ~= "table") then
			return
		end
		
		if (_T == nil) then
			_T = {}
		end
		_T["NAMESPACE"] = t
		_T["NAMESPACE"][#t] = nil
		_T["NAME"] = t[#t]
		
		package.loaders[1].StartCapture(t["next"])
	end,
	
	-- Handle inherits keyword
	Inherits = function(t)
		if (type(t) ~= "table") then
			return
		end
		
		_T["INHERITS"] = t
		
		package.loaders[1].StartCapture(t["next"])
	end,

	-- Variables and helper functions	
	BeginInspection = function(name)	
		-- Add the index metamethod to the environment.
		local f = function(t, key)
			local t = _G["_T"][name]
			t[#t + 1] = key
			getmetatable(_G)["__index"] = nil
			return setmetatable({}, { __index = f });
		end
		getmetatable(_G)["__index"] = f
		
		return
	end,
	
	-- Attempts to locate the actual pathname of the
	-- specified module
	GetPathName = function(module)
		local basepath, extpath, path, fhandle
	
		-- Replace the dots in the module name with
		-- slashes.
		basepath = String.Replace(module, "%.", "/")
		basepath = "./" .. basepath
		extpath = {"rcs", "rs", "rks", "lua"}
		path = nil
		
		for k, e in pairs(extpath) do
			fhandle = IO.Open(basepath .. "." .. e, "r")
			if (fhandle ~= nil) then
				fhandle:close()
				path = basepath .. "." .. e
				break
			end
		end
		
		return path
	end,
	
	-- Attempts to load a Lua chunk from the specified
	-- path name.
	GetChunk = function(path)
		local chunk, err
		
		-- Load the chunk or throw an exception
		chunk, err = LoadFile(path)
		if (chunk == nil) then
			print("Engine.InvalidSyntaxException: " .. err)
			--throw Engine.InvalidSyntaxException(err)
		end
		
		return chunk
	end,
	
	-- Seperates the class name and the namespace component
	-- from the specified module name.
	IsolateClassAndNamespace = function(module)
		local i
		local info = {}
	
		for i = #module, 0, -1 do
			if (String.Substring(module, i, i) == ".") then
				info["Class"] = String.Substring(module, i + 1)
				if (i ~= 0) then
					info["Namespace"] = String.Substring(module, 1, i - 1)
				else
					info["Namespace"] = ""
				end
				break
			end
		end
		
		return info
	end,
	
	-- Splits the specified string by a specified character
	SplitString = function(str, c)
		local result = {}
		
		for i = 1, String.Length(str), 1 do
			if (String.Substring(str, i, i) == c) then
				result[#result + 1] = String.Substring(str, 1, i - 1)
				str = String.Substring(module, i + 1)
				i = 1
			end
		end
		if (#str > 0) then
			result[#result + 1] = str
		end
		
		return result
	end,
	
	-- Run the chunk in a protected environment with the
	-- specialized keywords available.  Returns the value
	-- of sandbox and sandbox["_T"], where sandbox is the
	-- environment the chunk is run in.
	Sandbox = function(chunk, info)
		local Loader = package.loaders[1]
		
		local sandbox = {}
		sandbox["name"] = function(...) return Loader.Class(info, ...) end
		sandbox["inherits"] = function(...) return Loader.Inherits(info, ...) end
		sandbox["description"] = function(...) return Loader.Description(info, ...) end
		sandbox["attribute"] = setmetatable({}, Loader.Attribute)
		setmetatable(sandbox, {})
		package.loaders[1].StartCapture(sandbox)
		setfenv(chunk, sandbox)
		chunk()
		
		local _T = sandbox["_T"]
		sandbox["_T"] = nil
		
		return sandbox, _T
	end,
	
	-- Verify that the module is located at the correct location
	-- (NAMESPACE "." NAME == module), then create the namespaces
	-- with the first being located in the env table.
	VerifyAndCreateNamespaces = function(_T, env)
		if (_T["NAMESPACE"] ~= nil) then
			mpath = Table.Concat(_T["NAMESPACE"], ".") .. "." .. _T["NAME"]
			if (mpath ~= module) then
				print("ERR : Module name mismatch.  Loaded from " .. module .. ", but code specifies " .. mpath .. ".")
				return nil, "Module name mismatch.  Loaded from " .. module .. ", but code specifies " .. mpath .. "."
			end
		else
			mpath = _T["NAME"]
		end
		
		-- Now create the namespace tables if required.
		l = env
		if (_T["NAMESPACE"] ~= nil) then
			for k, n in pairs(_T["NAMESPACE"]) do
				l[n] = {}
				l = l[n]
			end
		end
	end,
	
	-- Copies the specified base class (the name of the base class is in
	-- _T["INHERITS"]) into the target class, wrapping the functions in
	-- the specified environment.  Returns the resultant class followed
	-- by the __base constructor if there is one.
	CopyBaseClass = function(_T, env)
		local Loader = package.loaders[1]
		
		-- Isolate the class name from the namespace component
		local info = Loader.IsolateClassAndNamespace(_T["INHERITS"]);
		local sandbox = {}
		local target = {}
		local __base = nil
		
		-- Run the package loader in a sandbox.
		package.loaders[1](_T["INHERITS"], sandbox)
		
		-- Get the inherited class table.
		local i = sandbox
		local ins = Loader.SplitString(info["Namespace"], ".")
		for k, v in pairs(ins) do
			i = i[v]
		end
		i = i[info["Class"] ]
		
		-- Now copy all of the inherited classes functions.
		for n, f in pairs(i) do
			if (n == "__init") then
				__base = f
				setfenv(__base, env)
			elseif (type(f) == "function") then
				target[n] = f
				setfenv(target[n], env)
			elseif (type(f) ~= "function") then
				target[n] = f
			end
		end
		
		return target, __base, i
	end
	
},{
	-- Metatable __call function is executed
	-- when a file needs to be included
	__call = function(self, module, env)
	
		-- Define our local variables
		local Loader = package.loaders[1]
		local chunk, info, sandbox, namespace
		local class, __init, __base, __parent
		local k, v, _T
		
		local path = Loader.GetPathName(module)
		
		-- Check to see whether the file was found
		if (path == nil) then
			print("Engine.PathNotFoundException: " .. path)
			--throw Engine.PathNotFoundException()
		end
		
		-- The file exists, so attempt to load it.
		chunk = Loader.GetChunk(path)
		
		-- Isolate the class name from the namespace component
		info = Loader.IsolateClassAndNamespace(module)
		
		-- Run the chunk in a protected environment with the
		-- specialized keywords available.
		sandbox, _T = Loader.Sandbox(chunk, info)
		if (_T == nil) then
			print("Engine.InvalidSyntaxException: No module information was specified for " .. module .. ".")
			--throw Engine.InvalidSyntaxException("No module information was specified for " .. module .. ".")
		end
		
		-- Verify that the module is located at the correct location (NAMESPACE "." NAME == module)
		namespace = Loader.VerifyAndCreateNamespaces(_T, env)
		
		-- Build the class.
		class = {}
		__init = nil
		__base = nil
		
		-- Copy the inheritance.
		if (_T["INHERITS"] ~= nil) then
			class, __base, __parent = Loader.CopyBaseClass(_T, env)
		end
		
		-- Copy the class's functions into it.
		for k, v in pairs(sandbox[info["Class"] ]) do
			if (k == "__init") then
				__init = v
				setfenv(__init, env)
			else
				class[k] = v
				setfenv(class[k], env)
			end
		end
		
		-- Check for static variables.
		for k, v in pairs(sandbox) do
			if (k ~= "inherits" and k ~= "name" and k ~= "description" and k ~= "author" and k ~= "lastmodified") then
				if (type(v) ~= "function") then
					-- Make the specified variable a static variable.
					class[n] = v
				end
			end
		end
		
		-- Build up the metatable for the class.
		local mt = {}
		mt.__call = function(class_tbl, ...)
			local obj = {}
			setmetatable(obj, class)
			if (__init ~= nil) then
				__init(obj, ...)
			elseif (__base ~= nil) then
				__base(obj, ...)
			end
			return obj
		end
		class.__index = class
		class.__init = __init
		class.__base = __base
		if (_T["INHERITS"] ~= nil) then
			class.__type = function(self)
				return { module, __parent }
			end
		else
			class.__type = function(self)
				return module
			end
		end
		setmetatable(class, mt)
		
		-- Now put the newly generated class in the namespace.
		namespace[info["Class"] ] = class
		
		return function()
		end
	end
})
]]