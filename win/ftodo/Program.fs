module cprintf =
    let cprintf c fmt = 
        Printf.kprintf 
            (fun s -> 
                let old = System.Console.ForegroundColor 
                try 
                  System.Console.ForegroundColor <- c
                  System.Console.Write s
                finally
                  System.Console.ForegroundColor <- old) 
            fmt
    let cprintfn c fmt = 
        cprintf c fmt
        printfn ""

open System
open cprintf

let help() =
    cprintfn ConsoleColor.DarkGray @" * usage:
    todo <command> <arguments>
  - connect - connect to ctodo library via nwapper (testing thing)
  - initdb - init empty database structure
  - set default database options without data lose, useful if you or some update broke it)
  - <msg> - just write todo <msg> to add new node to your todo list
      --first to put task on top priority
      --motivate - end todo note with additional word (see ending option)
  - read or r - to read all
  - edit or e <number> <msg> - edit task
  - mv <number1> <number2> - move task
  - rm <number> - delete task
  - clean - clean all tasks
  - swap or s <number1> <number2> - swap elements
  - sync - text synchronization to avoid binaries in vcs
  - set <option> <value> - todo options, available options:
      - syncdir - directory for vcs synchronization
      - syncfile - file for text serialization for synchronization (default 'readme.md')
        - git - execute git synchronization 1/0 for enable/disable (default 1)
        - hg - execute mercurial synchronization 1/0 for enable/disable (default 0)
        - svn - execute subversion synchronization 1/0 for enable/disable (default 0)
        - vv - execute veracity synchronization 1/0 for enable/disable (default 0)
      - end - always end todo notes with additional word (default 0)
      - ending - word, using for end feature (default 'be a man')"
let mutable username = "User"
let user() = cprintf ConsoleColor.DarkGreen "%s@ctodo# " username

open nwrap

let mutable t : todo = null
let mutable connected = false

t <- new todo()
connected <- true

let initdb() = 
                (* TESTING InitDB *)
    t.n_initdb();
let read() = 
                (* TESTING READ *)
    let todolist = t.n_read(0, 0);
    cprintf ConsoleColor.Red "+-------------------------------------------\n"
    todolist |> Seq.iteri(fun i s ->
        if i%2 = 0 then
            cprintf ConsoleColor.Blue "|%s|" s
        else
            cprintf ConsoleColor.Red " %s|" s 
            printfn "")
    cprintf ConsoleColor.Red "+-------------------------------------------\n"
let write p =
                (* TESTING WRITE *)
    if t.n_write([|"hello"; "world"|],2) = 0 then
        printfn "done"
    else
        printfn "error"

let console() = 
                (* FREAKING CONSOLE *)
    let rec eat (command : string) =
        command.Split(' ','(',')') 
        |> Seq.filter(fun s -> s.Length <> 0)
        |> fun C ->
            (Seq.head C).ToUpper() |> fun head ->
                let conn triller =
                    if connected then triller()
                    else cprintfn ConsoleColor.Red "Not connected to database"
                let checkparams needparams triller = 
                    if ( Seq.length C ) > needparams then triller() else 
                    cprintfn ConsoleColor.Red "Not enouth parameters for this command"
                let ifparams needparams triller1 triller2 = 
                    if ( Seq.length C ) > needparams then triller1() 
                    else triller2()
                if head = "QUIT" || head = "EXIT" then 0
                else
                    match head with 
                    | "INITDB"      -> conn <| fun() -> initdb()
                    | "READ"        -> conn <| fun() -> read()
                    | "WRITE"       -> conn <| fun() -> checkparams 1   <| fun () -> write (Seq.nth(1) C)
                    | "HELP"        -> help()
                    | _ -> ()
                    user(); eat( Console.ReadLine() )
    user()
    eat( Console.ReadLine() )

[<EntryPoint>]
let main argv = console()