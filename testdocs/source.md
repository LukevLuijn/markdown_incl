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

# header 1

## header 2

### header 3

#### header 4

##### header 5

!insert_url // place urls here

// document configuration

!ignore=//;             // this sets the ignore flag for inline comments like this one.
!assets=image,diagram;  // this sets the sub flags for asset indexing (no spaces).
!chap_num=true;         // chapter numbering, true/false

!enable_toc=true;               // enable toc
!depth_toc=4;                   // set depth of toc
!title_toc=Table of Content;    // set title of toc

!title_url=Literature;  // title of the url chapter.
!page_break=true;       // break page after every header 1.