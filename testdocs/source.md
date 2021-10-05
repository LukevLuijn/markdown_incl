!insert_toc                 // place toc here

# Sample document

This is a sample document.

|Sample image|
|:---:|
| <img width="200" src="https://previews.123rf.com/images/aquir/aquir1311/aquir131100316/23569861-sample-grunge-red-round-stamp.jpg"  alt="sample image"/> |
| !image - sample image|    // with the "!image" sub flag you don't have to number your assets, it happens automatically.

// you can site your resources anywhere in the document.
!url https://www.123rf.com/photo_23569861_sample-grunge-red-round-stamp.html


!sub sub_document.md        // include content of other documents.

!insert_url                 // place urls here

// document configuration

!ignore=//;                 // this sets the ignore flag for inline comments like this one.
!assets=image,diagram;      // this sets the sub flags for asset indexing (no spaces).
!numbers=true;              // chapter numbering, true/false
!toc=true,Table of content; // TOC true/false, title for chapter.
!url_title=Literature;      // title of the url chapter.
!page_break=true;           // break page after every header 1.