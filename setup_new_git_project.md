1. Create [a new repo](https://github.com/new)

* Repository name: `TestProject`
* [x] Public
* `Create repository`

2. Create a local project directory

```
    mkdir TestProject
    cd    TestProject
```

3. Initialize the repo

```
    git init
```

4. (Optional) Create and add a dummy file to the repo.:

```
    echo "# TestProject" >> README.md
    git add README.md
    git commit -m "Default readme"
```

5. Link the local repo to the remote repo.

``` 
    git remote add origin https://github.com/<USER>/<project>.git
    git push -u origin master
```

Git push is linking your local repository with a remote repository on branch `master`

Note:

* Origin is an ALIAS to the remote URL
* `master` is the default branch on the remote
* `-u` is an alias for `--set-upstream`

