import subprocess
import unittest
import os
import sys

app_folder = '/'.join(os.path.dirname(os.path.realpath(__file__)).split('/')[:-1])
app_name = 'proga'
app = app_folder + '/bin/Darwin/Release/' + app_name + ' '


def AreClose(actual,expected):
    return (actual <= expected + expected*0.05) and (actual >= expected - expected*0.05)

def AreVeryClose(actual,expected):
    return (actual <= expected + expected*0.0001) and (actual >= expected - expected*0.0001) or ((-actual <= -expected - expected*0.0001) and (-actual >= -expected + expected*0.0001))

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

def GetCoeficients(cmd, number):
    return list(map(float, cmd.split()[cmd.split().index('Coeficients')+2:cmd.split().index('Coeficients')+number+2]))


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
        expected = 'These are basic algorithms:\n\t1 - Const prediction\n\t2 - DecisionStump\n\t3 - K nearest neighbours\n\nTo build bagged algorithm type 2i, where i - number of basic algorithm\nTo build boosted algorithm type 3i, where i - number of basic algorithm\n\nTo get information about algorithm parameters and default values use command \'info <i>\', where i - number of algorithm\nAdditional algorithms:\n\t4 - Linear Regression\n\t5 - Decision Tree\n'
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


    def test_KNN_1_fold_2_n(self):
        cmd = 'ml ' + app_folder + '/../datasets/puma.csv 3 -fn 1 -a 2'
        res = GetResult(cmd)

        actualRmse = GetRmse(res)
        self.assertTrue(AreClose(actualRmse, 0.0183))

    def test_KNN_1_fold_7_n(self):
        cmd = 'ml ' + app_folder + '/../datasets/puma.csv 3 -fn 1 -a 7'
        res = GetResult(cmd)

        actualRmse = GetRmse(res)
        self.assertTrue(AreClose(actualRmse, 0.0238))

    def test_KNN_4_fold_2_n(self):
        cmd = 'ml ' + app_folder + '/../datasets/puma.csv 3 -fn 4 -a 2'
        res = GetResult(cmd)

        actualRmse = GetRmse(res)
        self.assertTrue(AreClose(actualRmse, 0.0318))

    def test_KNN_N209_full_traing_set(self):
        cmd = 'ml ' + app_folder + '/../datasets/cpu.small.csv 3 -a 209 -fn 1'
        res = GetResult(cmd)
        
        actualRmse = GetRmse(res)
        self.assertTrue(AreVeryClose(actualRmse, 160.4455))

    def test_KNN_N1_all_folds(self):
        cmd = 'ml ' + app_folder + '/../datasets/cpu.small.csv 3 -a 1 -fn 209'
        res = GetResult(cmd)
        
        actualRmse = GetRmse(res)
        self.assertTrue(AreClose(actualRmse, 61))

    def test_KNN_N77_all_folds(self):
        cmd = 'ml ' + app_folder + '/../datasets/cpu.small.csv 3 -a 77 -fn 209'
        res = GetResult(cmd)
        
        actualRmse = GetRmse(res)

        self.assertTrue(AreClose(actualRmse, 131))

    def test_LR_1_fold(self):
        cmd = 'ml ' + app_folder + '/../datasets/lr1.csv 4 -fn 1 -a 0.05,2000'
        res = GetResult(cmd)

        actualCoefs = GetCoeficients(res,2)
        actualRmse = GetRmse(res)

        self.assertTrue(AreVeryClose(actualCoefs[0], 0.3333333))
        self.assertTrue(AreVeryClose(actualCoefs[1], 0.6666666))
        self.assertEqual(actualRmse, 0)

    def test_LR_5_fold(self):
        cmd = 'ml ' + app_folder + '/../datasets/lr2.csv 4 -a 0.05,1337'
        res = GetResult(cmd)

        actualRmse = GetRmse(res)
        actualCoefs = GetCoeficients(res,2)
        
        self.assertTrue(AreVeryClose(actualRmse, 0.7645))
        self.assertTrue(AreVeryClose(actualCoefs[0], 0.9563))
        self.assertTrue(AreVeryClose(actualCoefs[1], -0.4078))

    def test_LR_5_fold_norm(self):
        cmd = 'ml ' + app_folder + '/../datasets/lr2.csv 4 -n 1'
        res = GetResult(cmd)

        actualRmse = GetRmse(res)   
        print(actualRmse)     
        self.assertTrue(AreVeryClose(actualRmse, 0.7645))


if __name__ == '__main__':
    unittest.main(argv=['first-arg-is-ignored'])
