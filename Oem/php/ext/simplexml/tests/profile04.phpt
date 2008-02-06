--TEST--
SimpleXML [profile]: Accessing a namespaced element
--SKIPIF--
<?php if (!extension_loaded("simplexml")) print "skip"; ?>
--FILE--
<?php 
$root = simplexml_load_string('<?xml version="1.0"?>
<root xmlns:reserved="reserved-ns">
 <reserved:child>Hello</reserved:child>
</root>
');

echo $root->children('reserved-ns')->child;
echo "\n---Done---\n";
?>
--EXPECT--
Hello
---Done--- 
