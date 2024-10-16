import time

class Gate:
    def __init__(self, name, gate_type, inputs, output, delay):
        self.name = name
        self.gate_type = gate_type
        self.inputs = inputs
        self.output = output
        self.delay = delay
        self.value = 0

    def compute_output(self):
        # Logic gate simulation based on gate type
        if self.gate_type == '&':
            self.value = int(all(self.inputs))
        elif self.gate_type == '|':
            self.value = int(any(self.inputs))
        elif self.gate_type == '~':
            self.value = int(not self.inputs[0])
        elif self.gate_type == '~(' + self.inputs[0] + '&' + self.inputs[1] ')':
            self.value = int(not all(self.inputs))
        elif self.gate_type == '~(' + self.inputs[0] + '|' + self.inputs[1] ')':
            self.value = int(not any(self.inputs))
        elif self.gate_type == '^':
            self.value = int(self.inputs[0] != self.inputs[1])
        elif self.gate_type == '~(' + self.inputs[0] + '^' + self.inputs[1] ')':
            self.value = int(self.inputs[0] == self.inputs[1])

        return self.value
