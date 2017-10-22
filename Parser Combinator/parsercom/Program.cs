using System;
using System.IO;

namespace parsercom
{
    class Program
    {
        static void Main(string []args)
        {
            Language lang = new Language();
            try
            {
                string[] s = System.IO.File.ReadAllLines(args[0]);
                string input = System.String.Join("", s);
                bool isPrettyPrint = false;
                try
                {
                    isPrettyPrint = (args[1] == "-print");
                }
                catch
                { }
                finally
                { 
                    lang.RunLangParser(input, isPrettyPrint);
                }
            }
            catch (IOException)
            {
                Console.WriteLine("cannot open file");
            }
            Console.Write("press key to exit");
            Console.ReadKey();
        }
    }
}
