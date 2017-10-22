using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

using Monad;
using Monad.Parsec;
using Monad.Parsec.Language;
using Monad.Parsec.Token;
using Monad.Parsec.Expr;
using Monad.Utility;

namespace parsercom
{
    class Language
    {            
        // Parser
        Parser<Term> integer;
        Parser<Term> variable;
        Parser<Term> E;
        Parser<Term> S;
        Parser<ImmutableList<Term>> top;

        public Language()
        {
            Parser<Term> exprlazy = null;
            Parser<Term> expr = Prim.Lazy<Term>(() => exprlazy);
            Func<Parser<Term>, Parser<Term>> contents;
            Func<Parser<Term>, Parser<ImmutableList<Term>>> many = Prim.Many;
            Func<Parser<Term>, Parser<Term>> @try = Prim.Try;

            var def = new Lang();
            var lexer = Tok.MakeTokenParser<Term>(def);
            var binops = BuildOperatorsTable<Term>(lexer);

            // Lexer
            var intlex = lexer.Integer;
            var parens = lexer.Parens;
            var semiSep = lexer.SemiSep;
            var identifier = lexer.Identifier;
            var reserved = lexer.Reserved;
            var reservedOp = lexer.ReservedOp;
            var whiteSpace = lexer.WhiteSpace;

            integer = (from n in intlex
                      select new Integer(n) as Term);

            variable = (from v in identifier
                       select new Var(v) as Term);

            contents = p =>
                from ws in whiteSpace
                from r in p
                select r;

            var subexpr = (from p in parens(expr)
                           select new Expression(p) as Term);

            E = (from f in @try(variable)
                        | @try(integer) 
                        | subexpr
                select f);

            S = (from r in reserved("read")
                 from v in variable
                 select new ReadOperator(v) as Term)
               | (from v in variable
                  from assign in reservedOp(":=")
                  from e in expr
                  select new AssignmentOp(v, e) as Term)
               | (from w in reserved("write")
                  from e in expr
                  select new WriteOperator(e) as Term)
               | (from s in reserved("skip")
                  select new SkipOperator() as Term)
               | (from w in reserved("while")
                  from e in expr
                  from d in reserved("do")
                  from t in top
                  from o in reserved("od")
                  select new WhileOperator(e, t) as Term)
               | (from r1 in reserved("if")
                  from e in expr
                  from r2 in reserved("then")
                  from t1 in top
                  from r3 in reserved("else")
                  from t2 in top
                  from r4 in reserved("fi")
                  select new Condition(e, t1, t2) as Term)
                 ;

            top = from ts in many(
                    from s in S
                    from semi in reservedOp(";")
                    select s
                  )
                  select ts;

            exprlazy = Ex.BuildExpressionParser<Term>(binops, E);
        }

        public void RunLangParser(string input, bool print_to_file = false)
        {
            var result = top.Parse(input);

            if (result.IsFaulted)
            {
                string errs = System.String.Join("\n",
                    result.Errors.Select(e =>
                        e.Message + "Expected " + e.Expected + " at " + e.Location +
                        " - " + e.Input.AsString().Substring(0, Math.Min(30, e.Input.AsString().Length))
                        + "...")
                    );
                Console.WriteLine(errs);
            }

            var resu = result.Value.First().Item1;
            var left = result.Value.First().Item2.AsString();
            if (left != "")
            {
                Console.WriteLine("errors");
            }
            Console.WriteLine("Program");
            foreach (Term n in resu)
            {
                printAstNode(n, 1);
                Console.WriteLine("  |--[;]");
            }

            if (print_to_file)
            {
                using (System.IO.StreamWriter file =
                       new System.IO.StreamWriter("out.txt"))
                {
                    foreach (Term n in resu)
                    {
                        prettyPrinter(n, 0, file);
                        file.WriteLine(";");
                    }
                }
            } 
        }

        private void printBorder(int deep)
        {
            /*Вывод пробелов*/
            for (int i = 0; i <= (deep - 1) * 2; i++)
            {
                Console.Write("  ");
            }
            Console.Write("|");
            for (int i = deep - 1; i < deep * 2; i++)
            {
                Console.Write("-");
            }
        }
        private void printAstNode(Term node, int deep)
        {
            printBorder(deep++);
            /*Вывод нетерминалов*/
            if (node.GetType() == typeof(Integer))
            {
                Console.WriteLine("Expr");
                printBorder(deep);
                var r = node as Integer;
                Console.WriteLine("[" + r.Value + "]");
            }
            if (node.GetType() == typeof(Var))
            {
                Console.WriteLine("X");
                printBorder(deep);
                var r = node as Var;
                string s = "[";
                foreach (var i in r.Id.Value)
                {
                    s += (i.Value);
                }
                s += "]";
                Console.WriteLine(s);
            }
            if (node.GetType() == typeof(SkipOperator))
            {
                Console.WriteLine("S");
                printBorder(deep);
                Console.WriteLine("[skip]");
            }
            if (node.GetType() == typeof(WriteOperator))
            {
                Console.WriteLine("S");
                printBorder(deep);
                Console.WriteLine("[write]");
                printAstNode((node as WriteOperator).Term, deep);
            }
            if (node.GetType() == typeof(ReadOperator))
            {
                Console.WriteLine("S");
                printBorder(deep);
                Console.WriteLine("[read]");
                printAstNode((node as ReadOperator).Ident, deep);
            }
            if (node.GetType() == typeof(BinaryOp))
            {
                var r = node as BinaryOp;
                Console.WriteLine("Expr");
                printAstNode(r.Lhs as Term, deep);
                string s = "[";
                foreach (var c in (r.Op as ReservedOpToken).Value)
                {
                    s += (c.Value);
                }
                s += "]";

                printBorder(deep);
                Console.WriteLine(s);
                printAstNode(r.Rhs as Term, deep);
            }
            if (node.GetType() == typeof(Condition))
            {
                var r = node as Condition;

                Console.WriteLine("S");
                printBorder(deep);

                Console.WriteLine("[if]");
                printAstNode(r.Expr as Term, deep);
                printBorder(deep);
                Console.WriteLine("[then]");
                foreach (Term n in r.then_body)
                {
                    printAstNode(n, deep);
                    printBorder(deep);
                    Console.WriteLine("[;]");
                }
                printBorder(deep);
                Console.WriteLine("[else]");
                foreach (Term n in r.else_body)
                {
                    printAstNode(n, deep);
                    printBorder(deep);
                    Console.WriteLine("[;]");
                }
                printBorder(deep);
                Console.WriteLine("[fi]");
            }
            if (node.GetType() == typeof(WhileOperator))
            {
                var r = node as WhileOperator;
                Console.WriteLine("S");
                printBorder(deep);
                Console.WriteLine("[while]");
                printAstNode(r.Expr as Term, deep);
                foreach (Term n in r.body)
                {
                    printAstNode(n, deep);
                    printBorder(deep);
                    Console.WriteLine("[;]");
                }
                printBorder(deep);
                Console.WriteLine("[od]");
            }
            if (node.GetType() == typeof(AssignmentOp))
            {
                var r = node as AssignmentOp;
                Console.WriteLine("S");
                printAstNode(r.Ident, deep);
                printBorder(deep);
                Console.WriteLine("[:=]");
                printAstNode(r.Expr as Term, deep);
            }
            if (node.GetType() == typeof(Expression))
            {
                var r = node as Expression;
                Console.WriteLine("E");
                printAstNode(r.Expr as Term, deep);
            }
        }

        private void printPrettyBorder(int deep, System.IO.StreamWriter file)
        {
            for (int i = 0; i < deep; i++)
            {
                file.Write("    ");
            }
        }
        private void prettyPrinter(Term node, int deep, System.IO.StreamWriter file)
        {
            /*Вывод нетерминалов*/
            if (node.GetType() == typeof(Integer))
            {
                var r = node as Integer;
                file.Write(r.Value);
            }
            if (node.GetType() == typeof(Var))
            {
                var r = node as Var;
                string s = "";
                foreach (var i in r.Id.Value)
                {
                    s += (i.Value);
                }
                file.Write(s);
            }
            if (node.GetType() == typeof(SkipOperator))
            {
                printPrettyBorder(deep, file);
                file.Write("skip");
            }
            if (node.GetType() == typeof(WriteOperator))
            {
                printPrettyBorder(deep, file);
                file.Write("write ");
                prettyPrinter((node as WriteOperator).Term, deep, file);
            }
            if (node.GetType() == typeof(ReadOperator))
            {
                printPrettyBorder(deep, file);
                file.Write("read ");
                prettyPrinter((node as ReadOperator).Ident, deep, file);
            }
            if (node.GetType() == typeof(BinaryOp))
            {
                var r = node as BinaryOp;
                prettyPrinter(r.Lhs as Term, deep, file);
                string s = "";
                foreach (var c in (r.Op as ReservedOpToken).Value)
                {
                    s += (c.Value);
                }
                file.Write(s);
                prettyPrinter(r.Rhs as Term, deep, file);
            }
            if (node.GetType() == typeof(Condition))
            {
                var r = node as Condition;
                printPrettyBorder(deep, file);
                file.Write("if ");
                prettyPrinter(r.Expr as Term, deep, file);
                file.Write("\n");
                printPrettyBorder(deep, file);
                file.WriteLine(" then");

                foreach (Term n in r.then_body)
                {
                    prettyPrinter(n, deep + 1, file);
                    file.WriteLine(";");
                }
                printPrettyBorder(deep, file);
                file.WriteLine("else");
                foreach (Term n in r.else_body)
                {
                    prettyPrinter(n, deep + 1, file);
                    file.WriteLine(";");
                }
                printPrettyBorder(deep, file);
                file.Write("fi");
            }
            if (node.GetType() == typeof(WhileOperator))
            {
                var r = node as WhileOperator;
                file.Write("while ");
                prettyPrinter(r.Expr as Term, deep, file);
                file.WriteLine(" do");
                foreach (Term n in r.body)
                {
                    prettyPrinter(n, deep + 1, file);
                    file.WriteLine(";");
                }
                printPrettyBorder(deep, file);
                file.Write("od");
            }
            if (node.GetType() == typeof(AssignmentOp))
            {
                var r = node as AssignmentOp;
                printPrettyBorder(deep, file);
                prettyPrinter(r.Ident, deep, file);
                file.Write(":=");
                prettyPrinter(r.Expr as Term, deep, file);
            }
            if (node.GetType() == typeof(Expression))
            {
                var r = node as Expression;
                prettyPrinter(r.Expr as Term, deep, file);
            }
        }

        private static OperatorTable<T> BuildOperatorsTable<T>(TokenParser<T> lexer)
            where T : Token
        {
            Func<T, T, ReservedOpToken, T> fn = (lhs, rhs, op) =>
            {
                return new BinaryOp(lhs, rhs, op) as T;
            };

            Func<ReservedOpToken, Func<T, T, T>> binop = op => new Func<T, T, T>((T lhs, T rhs) =>
            {
                return fn(lhs, rhs, op);
            });

            Func<string, Parser<Func<T, T, T>>> resOp = name => from op in lexer.ReservedOp(name) select binop(op);

            var equals = new Infix<T>("==", resOp("=="), Assoc.Left);
            var mult = new Infix<T>("*", resOp("*"), Assoc.Left);
            var divide = new Infix<T>("/", resOp("/"), Assoc.Left);
            var plus = new Infix<T>("+", resOp("+"), Assoc.Left);
            var minus = new Infix<T>("-", resOp("-"), Assoc.Left);
            var lessThan = new Infix<T>("<", resOp("<"), Assoc.Left);
            var lessOrEqualsThan = new Infix<T>("<=", resOp("<="), Assoc.Left);
            var neq = new Infix<T>("!=", resOp("!="), Assoc.Left);
            var mod = new Infix<T>("%", resOp("%"), Assoc.Left);
            var greater = new Infix<T>(">", resOp(">"), Assoc.Left);
            var geq = new Infix<T>(">=", resOp(">="), Assoc.Left);
            var and = new Infix<T>("&&", resOp("&&"), Assoc.Left);
            var or = new Infix<T>("||", resOp("||"), Assoc.Left);

            var binops = new OperatorTable<T>();
            binops.AddRow().Add(equals)
                  .AddRow().Add(mult).Add(divide)
                  .AddRow().Add(plus).Add(minus)
                  .AddRow().Add(lessThan)
                  .AddRow().Add(lessOrEqualsThan)
                  .AddRow().Add(neq)
                  .AddRow().Add(mod)
                  .AddRow().Add(greater)
                  .AddRow().Add(geq)
                  .AddRow().Add(and)
                  .AddRow().Add(or);
            return binops;
        }
    }

    public class Term : Token
    {
        public Term(SrcLoc location)
            :
            base(location)
        {
        }
    }

    public class Integer : Term
    {
        public int Value;
        public Integer(IntegerToken t, SrcLoc location = null)
            :
            base(location)
        {
            Value = t.Value;
        }
    }

    public class Var : Term
    {
        public IdentifierToken Id;

        public Var(IdentifierToken id, SrcLoc location = null)
            :
            base(location)
        {
            Id = id;
        }
    }

    public class BinaryOp : Term
    {
        public readonly Token Lhs;
        public readonly Token Rhs;
        public readonly Token Op;

        public BinaryOp(Token lhs, Token rhs, Token op, SrcLoc loc = null)
            :
            base(loc)
        {
            Lhs = lhs;
            Rhs = rhs;
            Op = op;
        }
    }

    public class SkipOperator : Term
    {
        public SkipOperator(SrcLoc location = null)
            :
            base(location)
        {
        }
    }

    public class WriteOperator : Term
    {
        public readonly Term Term;
        public WriteOperator(Term t, SrcLoc location = null)
            :
            base(location)
        {
            Term = t;
        }
    }

    public class ReadOperator : Term
    {
        public readonly Term Ident;
        public ReadOperator(Term i, SrcLoc location = null)
            :
            base(location)
        {
            Ident = i;
        }
    }

    public class AssignmentOp : Term
    {
        public readonly Term Ident;
        public Token Expr;
        public AssignmentOp(Term id, Token right, SrcLoc location = null)
            :
            base(location)
        {
            Ident = id;
            Expr = right;
        }
    }

    public class WhileOperator : Term
    {
        public Token Expr;
        public ImmutableList<Term> body;
        public WhileOperator(Token expr, ImmutableList<Term> t, SrcLoc location = null)
            :
            base(location)
        {
            Expr = expr;
            body = t;
        }
    }

    public class Condition : Term
    {
        public Token Expr;
        public ImmutableList<Term> then_body;
        public ImmutableList<Term> else_body;

        public Condition(Token expr, ImmutableList<Term> t1, ImmutableList<Term> t2, SrcLoc location = null)
            :
            base(location)
        {
            Expr = expr;
            then_body = t1;
            else_body = t2;
        }
    }

    public class Expression : Term
    {
        public Token Expr;
        public Expression(Token expr, SrcLoc location = null)
            :
            base(location)
        {
            Expr = expr;
        }
    }

    class Lang : EmptyDef
    {
        public Lang()
        {
            ReservedOpNames = new string[] { "+", "*", "-", ";", "/", "%", "<", ">", "==", "<=", ">=", ":=", "!=", "&&", "||"};
            ReservedNames = new string[] { "write", "read", "skip", "if", "then", "else", "fi", "while", "do", "od" };
            CommentLine = "#";
        }
    }
}
