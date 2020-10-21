import pandas as pd
import sys




def stat(path):

    df = pd.read_csv(path, encoding='utf-8')
    df = df.sort_values(by=['pH'])
    df = df.loc[df['pH'] != 2.74]


    df = df.sort_values(by=['chlorides'])

    print(df['quality'].value_counts())
    print(df)



if __name__ == "__main__":
    path = sys.argv[1]
    stat(path)
