-- Overwrite our loading functionality.
Package.Loaders = {}

Package.Loaders[1] = function(module, env)
	-- Declare our variables local so they don't
	-- interfere with the main program.
	local path, ext, found_path, e, fhandle
	local chunk, lmodule, _T, lfmodulepath
	local indent, l, n, ldotpos, classname
	local classpath
	local namespace, class_funcs, class, mt
	local __init, f, v, k, tdotpos, base, i
	local class_inheritance
	local class_visibility = {}
	local class_context = {}
	local inherit_classname, inherit_namespaces
	local inheritance_sandbox, i2
	local class_set = false
	local function func_inherits(class)
		class_inheritance = class
	end
	local function func_visibility(visibility)
		class_visibility = visibility
	end
	local function func_context(context)
		class_context = context
	end
	local function func_class(name)
		local ldotpos, tdotpos, classname, namespaces
		local z, x, c
		ldotpos = String.Length(name)
		tdotpos = String.Length(name)
		classpath = name
		classname = nil
		namespaces = {}
		while (ldotpos > 0) do
			if (String.Substring(name,ldotpos,ldotpos) == ".") then
				if (classname == nil) then
					classname = String.Substring(name, ldotpos + 1, tdotpos)
				else
					Table.Insert(namespaces, #namespaces + 1, String.Substring(name, ldotpos + 1, tdotpos))
				end
				tdotpos = ldotpos - 1
			end
			ldotpos = ldotpos - 1
		end
		if (tdotpos > 0) then
			if (classname == nil) then
				classname = String.Substring(name, ldotpos + 1, tdotpos)
			else
				Table.Insert(namespaces, #namespaces + 1, String.Substring(name, ldotpos + 1, tdotpos))
			end
		end
		z = {}
		for c, x in ipairs(namespaces) do
			Table.Insert(z, 1, x)
		end
		namespaces = z
		
		lmodule._T["CLASS"] = classname
		if (#namespaces > 0) then
			lmodule._T["NAMESPACE"] = namespaces
		else
			lmodule._T["NAMESPACE"] = nil
		end

		class_set = true
	end

	-- Define the environment and the class.
	class = {}
	base = {}
	if (env == nil) then
		env = setmetatable({}, {
			__index = function(tbl, key)
				if (key == "class") then
					return class
				elseif (key == "base") then
					return base
				else
					return _G[key]
				end
			end,
			__setindex = function(tbl, key, val)
				_G[key] = val
			end
		})
	end
	
	-- Replace the "." in the requested module with
	-- backslashes.
	path = String.Replace(module, "%.", "/")
	path = "./" .. path
	ext = {"rcs", "rs", "rks", "lua"}
	found_path = nil
	
	for k, e in pairs(ext) do
		fhandle = IO.Open(path .. "." .. e, "r")
		if (fhandle ~= nil) then
			fhandle:Close()
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
	ldotpos = String.Length(module)
	while (ldotpos > 0) do
		if (String.Substring(module,ldotpos,ldotpos) == ".") then
			break
		end
		ldotpos = ldotpos - 1
	end
	classname = String.Substring(module, ldotpos + 1)
	if (ldotpos ~= 0) then
		namespace = String.Substring(module, 1, ldotpos - 1)
	else
		namespace = ""
	end
	
	-- Run the chunk() function inside a sandbox, so we can inspect the module.
	lmodule = {}
	lmodule[classname] = {}
	lmodule["_T"] = {}
	lmodule["inherits"] = func_inherits
	lmodule["visibility"] = func_visibility
	lmodule["context"] = func_context
	lmodule["static"] = "static"
	lmodule["instance"] = "instance"
	lmodule["private"] = "private"
	lmodule["protected"] = "protected"
	lmodule["public"] = "public"
	setmetatable(lmodule, {
		__index = function(tbl, key)
			if (key == "class") then
				-- See whether we should serve up the function or
				-- a reference to the class object.
				if (class_set) then
					return class
				else
					return func_class
				end
			elseif (key == "base") then
				return base
			end

			return _G[key]
		end
		});
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
	_T["CLASS"]			= tostring(_T["CLASS"])
	
	-- Verify that the module is located at the correct location (NAMESPACE "." CLASS == module)
	if (_T["NAMESPACE"] ~= nil) then
		lfmodulepath = Table.Concat(_T["NAMESPACE"], ".") .. "." .. _T["CLASS"]
	else
		lfmodulepath = _T["CLASS"]
	end
	if (lfmodulepath ~= module) then
		print("ERR : Module name mismatch.  Loaded from " .. module .. ", but code specifies " .. lfmodulepath .. ".")
		return nil, "Module name mismatch.  Loaded from " .. module .. ", but code specifies " .. lfmodulepath .. "."
	end
	
	-- Now create the namespace tables if required.
	l = env
	if (_T["NAMESPACE"] ~= nil) then
		for k, n in pairs(_T["NAMESPACE"]) do
			if (l[n] == nil) then
				l[n] = {}
			end
			l = l[n]
		end
	end
	
	-- Get the class functions.
	class_funcs = lmodule[classname]
	lmodule[classname] = nil
	__init = nil
	__base = nil
	
	-- Build up the class.
	if (class_inheritance ~= nil) then
		local v = class_inheritance

		-- Isolate the class name from the namespace component
		ldotpos = String.Length(v)
		tdotpos = String.Length(v)
		inherit_classname = nil
		inherit_namespaces = {}
		while (ldotpos > 0) do
			if (String.Substring(v,ldotpos,ldotpos) == ".") then
				if (inherit_classname == nil) then
					inherit_classname = String.Substring(v, ldotpos + 1, tdotpos)
				else
					table.insert(inherit_namespaces,1,String.Substring(v, ldotpos + 1, tdotpos))
				end
				tdotpos = ldotpos - 1
			end
			ldotpos = ldotpos - 1
		end
		if (tdotpos > 0) then
			Table.Insert(inherit_namespaces,1,String.Substring(v, ldotpos + 1, tdotpos))
		end
		
		inheritance_sandbox = {}
		Package.Loaders[1](v, inheritance_sandbox)
		
		-- Load the class into i.
		i = inheritance_sandbox
		for i2, n in pairs(inherit_namespaces) do
			i = i[n]
		end
		i = i[inherit_classname]
		base = i

		-- Create an environment for the base class's functions
		-- which point the class variable to the inherited class.
		envi = setmetatable({}, {
				__index = function(tbl, key)
					if (key == "class") then
						return i
					elseif (key == "base") then
						return i.__base
					else
						return _G[key]
					end
				end,
				__setindex = function(tbl, key, val)
					_G[key] = val
				end
			})

		-- Now copy all of the inherited classes functions.
		for n, f in pairs(i) do
			if (type(f) == "function") then
				class[n] = f
				setfenv(class[n], envi)
			elseif (type(f) ~= "function") then
				class[n] = f
			end
		end
		for n, v in pairs(inheritance_sandbox) do
			if (n ~= "inherits"
				and n ~= "class"
				and n ~= "context"
				and n ~= "visibility"
				and n ~= "protected"
				and n ~= "instance"
				and n ~= "static"
				and n ~= "public"
				and n ~= "private") then
				if (type(v) == "function") then
					print("WARN: " .. lfmodulepath .. ":0: Function " .. n .. " defined without class context.  It is not included in the inherited class definition.")
				else
					-- Make the specified variable static if needed.
					if (class_context[n] == "static") then
						-- Make it a static variable.
						class[n] = v
					else
						-- Make sure that it's set up on each instance.
						if (class.__defaults == nil) then
							class.__defaults = {}
						end

						class.__defaults[n] = v
					end
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
			and n ~= "class"
			and n ~= "context"
			and n ~= "visibility"
			and n ~= "protected"
			and n ~= "instance"
			and n ~= "static"
			and n ~= "public"
			and n ~= "private") then
			if (type(v) == "function") then
				print("WARN: " .. lfmodulepath .. ":0: Function " .. n .. " defined without class context.  It is not included in the class definition.")
			else
				-- Make the specified variable static if needed.
				if (class_context[n] == "static") then
					-- Make it a static variable.
					class[n] = v
				else
					-- Make sure that it's set up on each instance.
					if (class.__defaults == nil) then
						class.__defaults = {}
					end

					class.__defaults[n] = v
				end
			end
		end
	end
	mt = {}
	mt.__type = { classpath, base };
	mt.__call = function(class_tbl, ...)
		local obj = {}
		setmetatable(obj, class)
		obj.__init = class.__init
		obj.__base = class.__base
		--obj.__type = class.__type

		-- Set up the defaults if needed.
		if (class.__defaults ~= nil) then
			for k, v in pairs(class.__defaults) do
				obj[k] = v
			end
		end

		if (obj.__init ~= nil) then
			obj:__init(...)
		else
			local b = obj
			while (b.__base ~= nil) do
				if (b.__base.__init ~= nil) then
					b.__base.__init(obj, ...)
					break
				else
					b = b.__base
				end
			end
		end
		return obj
	end
	class.__index = class
	class.__init = __init
	class.__base = base
	class.__type = mt.__type
	setmetatable(class, mt)
	
	-- Now put the newly generated class in the namespace.
	l[classname] = class
	
	return function()
	end
end






