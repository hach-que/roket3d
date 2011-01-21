-- Define our package loader.
require "_UnmanagedExt/ModuleLoader"

-- Test class imports
require "MainNamespace.ClassA"
require "AnotherNamespace.ClassB"

a = MainNamespace.ClassA();
b = AnotherNamespace.ClassB();