


# Sample document

This is a sample document.

|Sample image|
|:---:|
| <img width="200" src="https://previews.123rf.com/images/aquir/aquir1311/aquir131100316/23569861-sample-grunge-red-round-stamp.jpg"  alt="sample image"/> |
| !image - sample image| // with the "!image" sub flag you don't have to number your assets, it happens automatically.

// you can site your resources anywhere in the document.
!url 123RF. (n.d.). Stock Photo. Retrieved October 4, 2021, from https://www.123rf.com/photo_23569861_sample-grunge-red-round-stamp.html

// include content of other documents.
!sub sub_document.md

# Sample document

This is a sample document.

|Sample image|
|:---:|
| <img width="200" src="https://previews.123rf.com/images/aquir/aquir1311/aquir131100316/23569861-sample-grunge-red-round-stamp.jpg"  alt="sample image"/> |
| !image - sample image| // with the "!image" sub flag you don't have to number your assets, it happens automatically.

// you can site your resources anywhere in the document.
!url 123RF. (n.d.). Stock Photo. Retrieved October 4, 2021, from https://www.123rf.com/photo_23569861_sample-grunge-red-round-stamp.html

// include content of other documents.
!sub sub_document.md

// document configuration
!ignore=//; // this sets the ignore flag for inline comments like this one.
!assets=image,diagram; // this sets the sub flags for asset indexing (no spaces).
!numbers=true; // chapter numbering, true/false
!toc=true,table of content,2; // TOC true/false, title for chapter.
!url_title=test; // title of the url chapter.