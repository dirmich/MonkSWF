#!/usr/bin/ruby

# you should have swfmill installed (http://swfmill.org/)

svgfile = ARGV[0]

print "processing file #{svgfile}\n"

tmp_swfmill_import_file = File.new("tmp.xml", "w")

tmp_swfmill_import_file.puts("<movie width=\"320\" height=\"480\" framerate=\"12\">")
tmp_swfmill_import_file.puts("<background color=\"#ffffff\"/>")
tmp_swfmill_import_file.puts("<frame>")
tmp_swfmill_import_file.puts("<library>")
tmp_swfmill_import_file.puts("<clip id=\"square\" import=\"#{svgfile}\"/>")
tmp_swfmill_import_file.puts("</library>")
tmp_swfmill_import_file.puts("<place id=\"square\" name=\"mySquare\" x=\"0\" y=\"0\" depth=\"1\"/>")
tmp_swfmill_import_file.puts("</frame>")
tmp_swfmill_import_file.puts("</movie>")
tmp_swfmill_import_file.close

swfml_filename = svgfile.gsub(".svg", ".swfml")
swf_filename = svgfile.gsub(".svg", ".swf")
cmd = "swfmill simple tmp.xml #{swf_filename}"
puts cmd
system(cmd)
#{}%x[#\{cmd\}]
cmd = "swfmill swf2xml #{swf_filename} #{swfml_filename}"
puts cmd
system(cmd)
#{}%x[#\{cmd\}]
