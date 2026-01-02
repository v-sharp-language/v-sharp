package cli

import (
	"flag"
	"fmt"
	"os"
	"vsharp/internal/version"
)

type Command struct {
	Name        string
	Description string
	Run         func(fs *flag.FlagSet)
}

var commands []Command

func init() {
	commands = []Command{
		{
			Name:        "compile",
			Description: "Compile VSharp",
			Run: func(fs *flag.FlagSet) {
				fs.Parse(os.Args[2:])
				compileFile(os.Args[2])
			},
		},
		{
			Name:        "version",
			Description: "Print the VSharp compiler version.",
			Run: func(fs *flag.FlagSet) {
				fmt.Println(version.FullVersion())
			},
		},
	}
}
