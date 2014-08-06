OpenStack Python SDK Generator
==============================

This is a resource generator for the OpenStack Python SDK.  It was
not designed to be readable particularly.  It is some throw away
code that may be useful.

To run the generator, first put the sdkgen directory in your top
level sdk repository.  Next, create a file with the correct path
and at least contains the names of the fields you want in the
resource.  A simple example:

    ./sdkgen/t.sh sdkgen/network/v2/network

The format of the file is:

1) resource property name
2) API property name if the API uses a different name.  For example,
   the API may use tenant_id and the resource uses project_id.
3) Value to use for testing. If this is not specified, a string will
   be generated.

The script will start at the master branch, create a new branch
named 'network' and commit the two new files.  In this case it
will create these files:

    vi openstack/network/v2/network.py openstack/tests/network/v2/test_network.py

The easiest thing to do would be keep your files with just the fields
you want to add and edit them afterwards and add resource property
types, etc.  After edits:

    git commit -a --amend
