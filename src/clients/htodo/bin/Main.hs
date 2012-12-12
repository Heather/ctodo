import System.Environment ( getArgs )

import Hello
import Htodo

mainWorker :: [String] -> IO ()
mainWorker args = do
  putStrLn hello
  putStrLn todo

main :: IO ()
main = getArgs >>= mainWorker