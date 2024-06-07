void rapidity() {
    
    // example for Dp but needs to be changed for every folder
    
    const char* particle = "Dp";
    const char* inputName = Form("Gen%sMC.root", particle);
    const char* outputName = Form("Gen%sMC_output.root", particle);

    // Open the input ROOT file containing the original TTree
    TFile* input = new TFile(inputName, "READ");
    if (!input || input->IsZombie()) {
        std::cerr << "Error: Could not open input file " << inputName << std::endl;
        return;
    }

    // Get the original TTree
    TTree* input_tree = (TTree*)input->Get("MCDecayTreeTuple/MCDecayTree");
    if (!input_tree) {
        std::cerr << "Error: Could not find input TTree in the file" << std::endl;
        input->Close();
        return;
    }

    // Create a new ROOT file to store the output TTree
    TFile* output = new TFile(outputName, "RECREATE");
    if (!output || output->IsZombie()) {
        std::cerr << "Error: Could not create output file " << outputName << std::endl;
        input->Close();
        return;
    }

    // Clone the input TTree to create a new TTree with the same structure
    TTree* output_tree = input_tree->CloneTree(0); // Clone only structure, no data
    if (!output_tree) {
        std::cerr << "Error: Failed to clone input TTree" << std::endl;
        input->Close();
        output->Close();
        return;
    }

    int entries = input_tree->GetEntries();
    std::cout << "Number of entries in tree: " << entries << std::endl;
    double E=0.0, p_T=0.0, p_x=0.0, p_y=0.0, p_z=0.0, y=0.0, m;

    input_tree->SetBranchAddress(Form("%s_TRUEPT", particle), &p_T);
    input_tree->SetBranchAddress(Form("%s_TRUEP_E", particle), &E);
    input_tree->SetBranchAddress(Form("%s_TRUEP_Z", particle), &p_z);
    input_tree->SetBranchAddress(Form("%s_TRUEP_Y", particle), &p_y);
    input_tree->SetBranchAddress(Form("%s_TRUEP_X", particle), &p_x);
    output_tree->Branch(Form("%s_TRUEY", particle), &y, Form("%s_TRUEY/D", particle));

    TH1D *hist = new TH1D("hist", "Histogram", 1000, -0.1, 10);
    for (int i=0; i < entries; i++) {
        input_tree->GetEntry(i);
        y = 0.5 * TMath::Log((E+p_z)/(E-p_z));
        if (i == 0) {
            std::cout << "Mass of the particle found using E2-px2-py2-pz2 = " << TMath::Sqrt(E*E - p_x*p_x - p_y*p_y - p_z*p_z) << std::endl;
            std::cout << "P_X: " << p_x << std::endl;
            std::cout << "P_Y: " << p_y << std::endl;
            std::cout << "P_Z: " << p_z << std::endl;
            std::cout << "P_T: " << p_T << std::endl;
            std::cout << "E: " << E << std::endl;
            std::cout << "y = 0.5 ln((E+pz)/(E-pz)): " << y << std::endl;
        }
	hist->Fill(y);
        output_tree->Fill();
    }

    hist->Write();
    output_tree->Write();

    input->Close();
    output->Close();

    std::cout << "Rapidity calculation and tree creation completed successfully." << std::endl;
}


