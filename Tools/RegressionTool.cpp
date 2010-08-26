/*--
    RegressionTool.cpp  

    This file is part of the Cornucopia curve sketching library.
    Copyright (C) 2010 Ilya Baran (baran37@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "defs.h"
#include "RegressionTool.h"
#include "config.h"
#include "Regression.h"
#include "FormatTool.h"

#include <QDialog>
#include <QFileDialog>
#include <QDebug>
#include <QFileInfo>

using namespace std;
using namespace Eigen;

static const QString regressionFileName = QString(CORNUCOPIA_SOURCE_DIR) + "/Cornucopia/RegressionGen.cpp";

void RegressionTool::execute()
{
    QStringList files = QFileDialog::getOpenFileNames(parentWidget(), "Choose the datasets to combine",
                                                      QString(), "Cornu Dataset (*.cds)");

    Cornu::DatasetPtr dataSet = new Cornu::Dataset();
    for(int i = 0; i < (int)files.size(); ++i)
        dataSet->load(string(files[i].toAscii().constData(), files[i].length()));

    Cornu::DataModelPtr model = new Cornu::DataModel(dataSet);
    const std::vector<Eigen::VectorXd> &coefs = model->getCoefs();

    //now output
    {
        QFile file(regressionFileName);

        if(!file.open(QIODevice::WriteOnly))
        {
            qDebug() << "Unable to write file";
            return;
        }

        QTextStream os(&file);

        os << "//Automatically generated file -- DO NOT EDIT\n";
        os << "\n";
        os << "#include \"Regression.h\"\n";
        os << "\n";
        os << "using namespace Eigen;\n";
        os << "NAMESPACE_Cornu\n";
        os << "\n";
        os << "DataModel::DataModel()\n";
        os << "{\n";

        //Actual initialization
        os << "\t_coefs.resize(" << coefs.size() << ");\n";
        os << "\n";

        for(int i = 0; i < (int)coefs.size(); ++i)
        {
            if(coefs[i].size() == 0)
                continue;
            os << "\t_coefs[" << i << "].resize(" << coefs[i].size() << ");\n";
            os << "\t_coefs[" << i << "] << ";
            for(int j = 0; j < coefs[i].size(); ++j)
            {
                os << coefs[i][j];
                if( j + 1 < coefs[i].size())
                    os << ", ";
            }
            os << ";\n";
            os << "\n";
        }

        //finish up
        os << "}\n";
        os << "\n";
        os << "END_NAMESPACE_Cornu\n";
    }

    FormatTool::formatFile(regressionFileName);
}

#include "RegressionTool.moc"
