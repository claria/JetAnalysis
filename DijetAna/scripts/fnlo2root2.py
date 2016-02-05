#! /usr/bin/env python2
import sys
from fnlo_uncerts import fastNLOUncertainties
import code

import array
import ROOT
from ROOT import gDirectory
import numpy as np
ROOT.PyConfig.IgnoreCommandLineOptions = True


def main():


    # ./fnlo2root2.py ~/dust/FNLO/TRIPLEDIFF_YSB_PTAVG/triplediff.tab CT10nlo XSNLO_CT10.root
    # table = '/nfs/dust/cms/user/gsieber/FNLO/TRIPLEDIFF_YSB_PTAVG/triplediff.tab'
    # table = '/nfs/dust/cms/user/gsieber/dijetana/nlo/PTAVGEXPYS_YBYS_NLO_FINALBINS.tab'
    # output_file = 'PTAVGEXPYS_YBYS_NLO.root'
    table = sys.argv[1]
    output_file = sys.argv[2]

    pdfsets = {'CT10nlo' : 'CT10nlo', 'CT14nlo' : 'CT14nlo',
               'NNPDF30_nlo_as_0118' : 'NNPDF30', 
               'MMHT2014nlo68cl' : 'MMHT2014',
               'abm11_5n_nlo' :'ABM11NLO',
               'abm12lhc_5_nnlo' : 'ABM12NNLO',
               # 'HERAPDF20_NLO_EIG' : 'HERAPDF20NLO'
               }

    f = ROOT.TFile(output_file, 'RECREATE')
    for pdfset, pdfsetlabel in pdfsets.items():

        if pdfset in ['CT10nlo', 'CT14nlo']:
            pdf_clscale = 1.645
        else:
            pdf_clscale = None
        fnlo = fastNLOUncertainties(table, pdfset, pdf_clscale=pdf_clscale)
        xs = fnlo.get_central_crosssection()
        scale_unc = fnlo.get_scale_uncert()
        pdf_unc = fnlo.get_pdf_uncert()

        fnlo_orig = fnlo._fnlo
        k_factor = fnlo_orig.GetKFactors()

        diff0_bins = [0.0, 1.0, 2.0, 3.0]
        diff1_bins = [0.0, 1.0, 2.0, 3.0]
        diff2_bins = [34, 42, 50, 58, 66, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1784, 2116, 2500, 3000]

        diff0_bins_arr = array.array('d', diff0_bins)
        diff1_bins_arr = array.array('d', diff1_bins)
        diff2_bins_arr = array.array('d', diff2_bins)


        for i in range(len(diff0_bins) -1):
            for j in range(len(diff1_bins) -1):
                foldername = "yb{0}ys{1}".format(i, j)
                f.cd('/')
                if ROOT.gDirectory.GetDirectory(output_file) == None:
                    f.mkdir(foldername)
                f.cd("/" +foldername)

                histo_xsnlo = ROOT.TH1D('{0}_xs'.format(pdfsetlabel), '{0}_xs'.format(pdfsetlabel), len(diff2_bins_arr)-1, diff2_bins_arr) 
                histo_xsnlo.Sumw2()
                histo_kfac = ROOT.TH1D('{0}_kfac'.format(pdfsetlabel), '{0}_kfac'.format(pdfsetlabel), len(diff2_bins_arr)-1, diff2_bins_arr) 
                histo_kfac.Sumw2()

                histo_pdf_lo = ROOT.TH1D('{0}_pdfunc_l'.format(pdfsetlabel), '{0}_pdfunc_l'.format(pdfsetlabel), len(diff2_bins_arr)-1, diff2_bins_arr) 
                histo_pdf_lo.Sumw2()
                histo_pdf_up = ROOT.TH1D('{0}_pdfunc_u'.format(pdfsetlabel), '{0}_pdfunc_u'.format(pdfsetlabel), len(diff2_bins_arr)-1, diff2_bins_arr) 
                histo_pdf_up.Sumw2()
                histo_sc_lo = ROOT.TH1D('{0}_scunc_l'.format(pdfsetlabel), '{0}_scunc_l'.format(pdfsetlabel), len(diff2_bins_arr)-1, diff2_bins_arr) 
                histo_sc_lo.Sumw2()
                histo_sc_up = ROOT.TH1D('{0}_scunc_u'.format(pdfsetlabel), '{0}_scunc_u'.format(pdfsetlabel), len(diff2_bins_arr)-1, diff2_bins_arr) 
                histo_sc_up.Sumw2()
                histo_tot_lo = ROOT.TH1D('{0}_tot_l'.format(pdfsetlabel), '{0}_tot_l'.format(pdfsetlabel), len(diff2_bins_arr)-1, diff2_bins_arr) 
                histo_tot_lo.Sumw2()
                histo_tot_up = ROOT.TH1D('{0}_tot_u'.format(pdfsetlabel), '{0}_tot_u'.format(pdfsetlabel), len(diff2_bins_arr)-1, diff2_bins_arr) 
                histo_tot_up.Sumw2()

                dim1_bounds = fnlo_orig.GetObsBinsBounds(0)
                dim0_bounds = fnlo_orig.GetObsBinsBounds(1)
                dim2_bounds = fnlo_orig.GetObsBinsBounds(2)
                # print dim0_bounds
                # print dim1_bounds
                # print dim2_bounds

                for k in range(fnlo_orig.GetNObsBin()):
                    # print fnlo_orig.GetBinBoundaries(i)
                    if (dim0_bounds[k][0] == diff1_bins[j] and dim0_bounds[k][1] == diff1_bins[j+1] 
                       and dim1_bounds[k][0] == diff0_bins[i] and dim1_bounds[k][1] == diff0_bins[i+1]):
                        print "id bins"
                        print dim0_bounds[k][0], diff0_bins[i] 
                        print dim0_bounds[k][1], diff0_bins[i+1] 

                        print np.mean(dim2_bounds[k])
                        ibin = histo_xsnlo.FindBin(np.mean(dim2_bounds[k]))
                        if xs[k] < 1E-12:
                            xs[k] = 0.
                        histo_xsnlo.SetBinContent(ibin, xs[k])
                        histo_xsnlo.SetBinError(ibin, 0.01 * xs[k])
                        histo_kfac.SetBinContent(ibin, k_factor[k])
                        histo_kfac.SetBinError(ibin, 0.00)
                        histo_pdf_lo.SetBinContent(ibin, xs[k] - pdf_unc[0][k])
                        histo_pdf_up.SetBinContent(ibin, xs[k] + pdf_unc[1][k])
                        histo_sc_lo.SetBinContent(ibin, xs[k] - scale_unc[0][k])
                        histo_sc_up.SetBinContent(ibin, xs[k] + scale_unc[1][k])
                        histo_tot_lo.SetBinContent(ibin, xs[k] - np.sqrt((scale_unc[0][k])**2 + (pdf_unc[0][k])**2))
                        histo_tot_up.SetBinContent(ibin, xs[k] + np.sqrt((scale_unc[1][k])**2 + (pdf_unc[1][k])**2))


                histo_xsnlo.ComputeIntegral()
                histo_kfac.ComputeIntegral()
                histo_pdf_lo.ComputeIntegral()
                histo_pdf_up.ComputeIntegral()
                histo_sc_lo.ComputeIntegral()
                histo_sc_up.ComputeIntegral()

                histo_xsnlo.Write()
                histo_kfac.Write()
                histo_pdf_lo.Write()
                histo_pdf_up.Write()
                histo_sc_lo.Write()
                histo_sc_up.Write()
                histo_tot_lo.Write()
                histo_tot_up.Write()

                del histo_xsnlo
                del histo_pdf_lo
                del histo_pdf_up
                del histo_sc_lo
                del histo_sc_up
                del histo_tot_lo
                del histo_tot_up

    f.Close()


def flatten(lst):
    return [val for sublist in lst for val in sublist]

def f7(seq):
    seen = set()
    seen_add = seen.add
    return [ x for x in seq if not (x in seen or seen_add(x))]

if __name__ == '__main__':
    main()

