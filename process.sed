s/^\/\/@ *\([a-zA-Z0-9\/\.]*\)/cat \1/e # Replace //@ filename.txt with filename.txt contents
/^\/\/!.*$/d                            # Delete lines starts with //!