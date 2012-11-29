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
open System.Configuration

open System.Linq
open cprintf

let help() =
    cprintfn ConsoleColor.Yellow @" * usage:
    todo <command> <arguments>
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
let connect address _username = 
    try 
        t <- new todo()
        connected <- true
    with | _ as errorM -> printfn "%s" errorM.Message
let read() = 
    let todolist = t.n_read(0, 0);
    for s in todolist do
        cprintf ConsoleColor.Blue "%s" s
        printfn ""

let console() = 
    let rec eat (command : string) =
        command.Split(' ','(',')') 
        |> Seq.filter(fun s -> s.Length <> 0)
        |> fun C ->
            (Seq.head C).ToUpper() |> fun head ->
                let conn triller =
                    if connected then triller()
                    else cprintfn ConsoleColor.Red "Not connected to server"
                let checkparams needparams triller = 
                    if ( Seq.length C ) > needparams then triller() else 
                    cprintfn ConsoleColor.Red "Not enouth parameters for this command"
                let ifparams needparams triller1 triller2 = 
                    if ( Seq.length C ) > needparams then triller1() 
                    else triller2()
                if head = "QUIT" || head = "EXIT" then 
                    if connected then (); 
                    0
                else
                    match head with 
                    | "CONNECT" -> ifparams 2 <| fun () -> connect (Seq.nth(1) C) (Seq.nth(2) C)
                                              <| fun () -> connect "" ""
                    | "READ"        -> conn   <| fun() -> read()
                    | "HELP"        -> conn   <| fun() -> help()
                    | _ -> ()
                    user(); eat( Console.ReadLine() )
    user()
    eat( Console.ReadLine() )

[<EntryPoint>]
let main argv = console()