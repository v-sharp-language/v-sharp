package cli

import "flag"

func RunCLI(args []string) {
	if len(args) < 2 {
		return
	}

	cmdName := args[1]

	for _, cmd := range commands {
		if cmd.Name == cmdName {
			fs := flag.NewFlagSet(cmd.Name, flag.ExitOnError)
			cmd.Run(fs)
			return
		}
	}
}
