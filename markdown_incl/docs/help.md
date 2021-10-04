!assets image figure

This flag should be placed at the top of your source file,
it will replace every set key (in this case "!image" and "!figure") with "Image N"
and "Figure M" where N is the current image index and M the current figure index.
you can use these sub-flags in any included document.

!sub sub_document.md

This flag will search in the local directory for document 'sub_document.md'
and place it's content at the location of the flag.
you can use this in any included document.

!url www.someurl.com/something/else

This flag will take this URL and place it at the bottom of the output file. 
This flag can also be used for other things than URLs of course.
you can use this in any included document.

