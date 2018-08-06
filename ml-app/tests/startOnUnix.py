import subprocess
import unittest
import os
import sys

curConfiguration = sys.argv[1]

app_folder = '/'.join(os.path.dirname(os.path.realpath(__file__)).split('/')[:-1])
app_name = 'proga'
app = app_folder + '/build/Unix/'+ curConfiguration + '/' + app_name + ' '


def AreClose(actual,expected):
    return (actual <= expected + expected*0.015) and (actual >= expected - expected*0.015)

def AreVeryClose(actual,expected):
    return (actual <= expected + expected*0.0001) and (actual >= expected - expected*0.0001)

def GetResult(cmd):
    return subprocess.check_output(app+cmd, shell = True)

def GetRmse(cmd):
    return float(cmd.split()[cmd.split().index('RMSE')+1])

def GetValueConstPrediction(cmd):
    return float(cmd.split()[cmd.split().index('=')+1])

def GetFoldsNumber(cmd):
    return float(cmd.split()[cmd.split().index('Folds')+2])

def GetThreshold(cmd):
    return float(cmd.split()[cmd.split().index('Threshold')+1])

def GetValueBelow(cmd):
    return float(cmd.split()[cmd.split().index('below')+1])

def GetValueAbove(cmd):
    return float(cmd.split()[cmd.split().index('above')+1])


# +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
# +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
# +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

class SimpleCommands(unittest.TestCase):

    def test_Is_alive(self):
        actual = GetResult('')
        expected = 'Type \"' + app_name + ' help\" for help\n'
        self.assertEqual(actual, expected)


    def test_Help_command(self):
        cmd = 'help'
        actual = GetResult(cmd)
        expected = '\nUsage format:\n\t' + app_name + ' <command> <arguments> \n\nCommands:\n\thelp <command name> - get help about command\n\talgs - get list of available algorithms\n\tinfo <number of algorithm> - get information about arguments and default values of algorithm\n\tml - get estimation of algorithm on dataset, see "ml" command format using info\n'
        self.assertEqual(actual,expected)


    def test_Algs_command(self):
        cmd = 'algs'
        actual = GetResult(cmd)
        expected = 'These are basic algorithms:\n\t1 - Const prediction\n\t2 - DecisionStump\n\t3 - K nearest neighbours\n\nTo build bagged algorithm type 2i, where i - number of basic algorithm\nTo build boosted algorithm type 3i, where i - number of basic algorithm\n\nTo get information about algorithm parameters and default values use command \'info <i>\', where i - number of algorithm\n'
        self.assertEqual(actual,expected)

    def test_Info_command(self):
        cmd = 'info'
        exp1 = 'Const prediction has no parameters\n'
        exp2 = 'DecisionStump has no parameters\n'
        exp3 = 'K nearest neighbours has 1 parameter K, default value is 5\n'
        exp2i = 'First parameter - number of models to be bagged, default value is 25\nSecond parameter - size of training set, default = 0.8 * <size of dataset>\n'
        exp3i = 'First parameter - number of models to be boosted, default value is 50\n'

        self.assertEqual(GetResult(cmd+' 1'), exp1)
        self.assertEqual(GetResult(cmd+' 2'), exp2)
        self.assertEqual(GetResult(cmd+' 3'), exp3)

        self.assertEqual(GetResult(cmd+' 21'), exp2i+exp1)
        self.assertEqual(GetResult(cmd+' 22'), exp2i+exp2)
        self.assertEqual(GetResult(cmd+' 23'), exp2i+exp3)

        self.assertEqual(GetResult(cmd+' 31'), exp3i+exp1)
        self.assertEqual(GetResult(cmd+' 32'), exp3i+exp2)
        self.assertEqual(GetResult(cmd+' 33'), exp3i+exp3)


# +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


class AlgorithmScore(unittest.TestCase):

    def test_default_folds_number_and_ConstPrediction(self):
        cmd = 'ml ' + app_folder + '/../datasets/cpu.small.csv 1'
        res = GetResult(cmd)

        actualRmse = GetRmse(res)
        actualPred = GetValueConstPrediction(res)
        actualFolds = GetFoldsNumber(res)

        self.assertTrue(AreClose(actualRmse, 161))
        self.assertTrue(AreVeryClose(actualPred, 105.622009569378))
        self.assertEqual(actualFolds, 5)


    def test_argument_folds_number_and_ConstPrediction(self):
        cmd = 'ml ' + app_folder + '/../datasets/cpu.small.csv 1 -fn 20'
        res = GetResult(cmd)

        actualRmse = GetRmse(res)
        actualPred = GetValueConstPrediction(res)
        actualFolds = GetFoldsNumber(res)

        self.assertTrue(AreClose(actualRmse, 161))
        self.assertTrue(AreVeryClose(actualPred, 105.622009569378))
        self.assertEqual(actualFolds, 20)


    def test_DecisionStump_full_training_set(self):
        cmd = 'ml ' + app_folder + '/../datasets/cpu.small.csv 2 -fn 1'
        res = GetResult(cmd)

        actualRmse = GetRmse(res)
        actualValueBelow = GetValueBelow(res)
        actualValueAbove = GetValueAbove(res)
        actualValueThreshold = GetThreshold(res)

        self.assertTrue(AreVeryClose(actualRmse, 107.0416))
        self.assertTrue(AreVeryClose(actualValueBelow, 88.92682926829268))
        self.assertTrue(AreVeryClose(actualValueAbove, 961.25))
        self.assertTrue(AreVeryClose(actualValueThreshold, 48000.0))


    def test_DecisionStump_all_folds(self):
        cmd = 'ml ' + app_folder + '/../datasets/cpu.small.csv 2 -fn 209'
        res = GetResult(cmd)

        actualRmse = GetRmse(res)
        actualValueBelow = GetValueBelow(res)
        actualValueAbove = GetValueAbove(res)
        actualValueThreshold = GetThreshold(res)
        actualFoldsNumber = GetFoldsNumber(res)

        self.assertTrue(AreVeryClose(actualRmse, 136.2801))
        self.assertTrue(AreVeryClose(actualValueBelow, 88.92682926829268))
        self.assertTrue(AreVeryClose(actualValueAbove, 961.25))
        self.assertTrue(AreVeryClose(actualValueThreshold, 48000.0))
        self.assertEqual(actualFoldsNumber, 209)


if __name__ == '__main__':
    unittest.main(argv=['first-arg-is-ignored'])