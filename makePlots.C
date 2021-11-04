void makePlots(int run)
{
gStyle->SetPalette(kInvertedDarkBodyRadiator);
gStyle->SetOptStat("nei");

TFile *builtFile = TFile::Open(Form("run_%d_builtFile.root",run));
TTree *builtTree = (TTree*)(builtFile->Get("builtTree"));

builtTree->Draw("A_E>>ae(800,0,4096)","A_E!=-1 && B_E!=-1 && C_E!=-1 && D_E!=-1","goff");
builtTree->Draw("B_E>>be(800,0,4096)","A_E!=-1 && B_E!=-1 && C_E!=-1 && D_E!=-1","goff");
builtTree->Draw("C_E >>ce(800,0,4096)","A_E!=-1 && B_E!=-1 && C_E!=-1 && D_E!=-1","goff");
builtTree->Draw("D_E*40 >>de(800,0,4096)","A_E!=-1 && B_E!=-1 && C_E!=-1 && D_E!=-1","goff");


TH1D* xyz1 = (TH1D*)(gDirectory->Get("ae"));
TH1D* xyz2 = (TH1D*)(gDirectory->Get("be"));
TH1D* xyz3 = (TH1D*)(gDirectory->Get("ce"));
TH1D* xyz4 = (TH1D*)(gDirectory->Get("de"));
xyz1->SetLineColor(kBlack);
xyz2->SetLineColor(kBlue);
xyz3->SetLineColor(kRed);
xyz4->SetLineColor(kGreen);

TCanvas *c1 = new TCanvas("at least A","",0,0,600,600);
c1->cd();
xyz1->Draw();
xyz2->Draw("same");
xyz3->Draw("same");
xyz4->Draw("same");
c1->Modified();
c1->Update();
c1->BuildLegend();

/*
builtTree->Draw("A_E:B_E>>ab(800,0,4096,800,0,4096)","A_E!=-1 && B_E!=-1","goff");
builtTree->Draw("A_E:C_E>>ac(800,0,4096,800,0,4096)","A_E!=-1 && C_E!=-1","goff");
builtTree->Draw("B_E:C_E>>bc(800,0,4096,800,0,4096)","B_E!=-1 && C_E!=-1","goff");

TH2D* ab = (TH2D*)(gDirectory->Get("ab"));
TH2D* ac = (TH2D*)(gDirectory->Get("ac"));
TH2D* bc = (TH2D*)(gDirectory->Get("bc"));

TCanvas *c2 = new TCanvas("2d plots","",600,0,600,600);
c2->cd();
c2->Divide(1,3);
c2->cd(1);
ab->Draw("colz");
c2->cd(2);
ac->Draw("colz");
c2->cd(3);
bc->Draw("colz");
c2->Modified();
c2->Update();
c2->BuildLegend();
*/



}
