declare variable $projectFile external;
declare variable $projectDoc := doc($projectFile);

declare variable $mainProject := "xqilla";
declare variable $testProjects := ();

declare function local:fileLine($file)
{
  let $project := $file/ancestor::project
  return (
    "&#10;", string($file/@name),
    if($project/@name = $mainProject) then " lib"
    else if($project/@name = $testProjects) then (" test=", $project/@name)
    else if($project/type = "library") then (" dll=", $project/@name)
    else if($project/type = "app") then (" app=", $project/@name)
    else error(QName("", "error"), "Unknown project type")
  )
};

string-join((
"# $Id$
#
# Generated by dspproject.xq from xqilla.template.xml ** DO NOT EDIT **
#
# This is an input file for scripts to build Windows projects.
# It lists the source files in the XQilla tree and notes which are 
# used to build the Windows libraries.
#
# Please keep this list sorted alphabetically!
#
# Each non-blank, non-comment line is of the form
#   filename  module [ module ...]
#
# A single file can be used in more than one module, if appropriate, but
# this is not typical.
#
# The possible modules:
#
#   app=NAME Linked into application NAME.exe (NAME.{dsp,vcproj})
#   dll=NAME Linked into dll NAME.dll (xqilla_NAME.{dsp,vcproj})
#   lib  File is in the main Windows DLL/LIB (xqilla.{.dsp,vcproj})
#
# Windows projects may be arranged hierarchically, with sub-folders.
# The project generation mechanism supports one level of hierarchy
# (more could be added, but work is required).
# If hierarchy is desired, add a line ""module_name.filters=space separated 
# folders"":
# o folder names must match exactly and uniquely a directory in 
# the module's source listing
# o all matching files will be placed in that folder
# 
"
,
"lib.filters=", string-join(
  for $f in distinct-values($projectDoc//filter/@name)
  where $f != "include"
  order by $f
  return $f, " ")
,
for $f in $projectDoc//file
where not($f/parent::filter/@name = "include")
order by lower-case($f/@name)
return local:fileLine($f)
,
for $f in $projectDoc//file
where $f/parent::filter/@name = "include"
order by lower-case($f/@name)
return local:fileLine($f)
), "")
