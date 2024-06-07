void openInputFiles(std::vector<TGraphAsymmErrors*>& graphs, bool Dst = false, bool indirect = false, bool alice = false) {
    // boolean params are: false/true (for D0/Dst), false/true (for ccbar/other sources), false/true (for lchb/alice) respectively

    std::string fileName;
    if (alice) {
        if (indirect) {
            if (Dst) {
                fileName = "alice_other_plotratio_Dstar_Dp.root";
            } else {
                fileName = "alice_indirect_from_Dst_D0_Dp.root";
            }
        } else {
            if (Dst) {
                fileName = "alice_direct_hadronization_plotratio_Dstar_Dp.root";
            } else {
                fileName = "alice_direct_hadronization_plotratio_D0_Dp.root";
            }
        }
    } else {
        if (indirect) {
            if (Dst) {
                fileName = "plotratio_other_Dstar_Dp.root";
            } else {
                fileName = "indirect_from_Dst_D0_Dp.root";
            }
        } else {
            if (Dst) {
                fileName = "plotratio_direct_hadronization_Dstar_Dp.root";
            } else {
                fileName = "plotratio_direct_hadronization_D0_Dp.root";
            }
        }
    }

    auto file = new TFile(fileName.c_str(), "READ");

    if (!file || file->IsZombie()) {
        std::cerr << "Error: Could not open input file " << fileName << "." << std::endl;
        delete file; 
        file = nullptr;
        return;
    }

    int numGraphs = (alice ? 1 : 2);

    for (size_t i = 0; i < numGraphs; ++i) {
        std::string graphName = "graph" + (alice ? "" : "_" + std::to_string(i));
        TGraphAsymmErrors* newGraph = (TGraphAsymmErrors*)file->Get(graphName.c_str());
        graphs.push_back(newGraph);
    }

    std::cout << "Input file " << fileName << " read successfully." << std::endl;
}

void bla_bla() {

    std::vector<int> graphs;
    graphs.push_back(1);
    graphs.push_back(2);
    graphs.push_back(3);
    std :: cout << graphs[0] << graphs[1] << graphs[2];
}
