/*
 * Filter Coefficients (C Source) generated by the Filter Design and Analysis Tool
 * Generated by MATLAB(R) 9.7 and Signal Processing Toolbox 8.3.
 * Generated on: 11-Nov-2020 15:07:56
 */

/*
 * Discrete-Time FIR Filter (real)
 * -------------------------------
 * Filter Structure  : Direct-Form FIR
 * Filter Length     : 406
 * Stable            : Yes
 * Linear Phase      : Yes (Type 2)
 */

/* General type conversion for MATLAB generated C-code  */
#include "tmwtypes.h"
/* 
 * Expected path to tmwtypes.h 
 * /home/ma/MATLAB/R2019b/extern/include/tmwtypes.h 
 */
const int FIR_1600_BL = 406;
const real64_T FIR_1600_B[406] = {
  -0.0001153864590388,-1.315710194372e-05,6.127583862425e-05,6.876685924578e-05,
  -2.202267065696e-05,-9.394869692614e-05,-2.943363024564e-05,0.0001032497032984,
   0.000113599637106,-4.11342835417e-05,-0.0001579112012099,-5.08446917726e-05,
  0.0001591173176174,0.0001723657362514,-6.540223339257e-05,-0.0002383431214105,
  -7.723146575159e-05,0.0002278909321653,0.0002435803737285,-9.398785474977e-05,
  -0.0003325903960892,-0.0001075779696676,0.0003064171372759,0.0003237039064647,
  -0.0001254760228883,-0.0004349409006286,-0.0001398922945606,0.0003890804726499,
    0.00040649407191,-0.0001572049173897,-0.0005363804029423,-0.0001712456510351,
  0.0004673966463355,0.0004829964454818,-0.000185627683138,-0.0006245171573531,
  -0.0001975475974528,0.0005303135082512,0.0005415237605539,-0.0002061474994892,
  -0.0006838020663353,-0.0002138516919861,0.0005643192687603, 0.000568276521277,
  -0.0002132927053908,-0.0006962682863811,-0.0002144989224296,0.0005543764866565,
  0.0005479261554693,-0.0002012316940175,-0.0006426752026104,-0.0001934536048555,
  0.0004848413160558, 0.000464857450957,-0.0001640395580874,-0.0005040721510313,
  -0.0001450072079932,0.0003408589216372,0.0003046044125928,-9.634528249988e-05,
  -0.0002637057084412,-6.41663240665e-05,0.0001100619362646, 5.54511957325e-05,
  5.954819118623e-06,9.084769417943e-05,5.258399473393e-05,-0.0002156600448077,
  -0.0002896650877668,0.0001451343680556,0.0005654166939372, 0.000206541029895,
  -0.0006384303045448,-0.0007314272529938,0.0003209210248809, 0.001157162142485,
  0.0003962072043767,-0.001152699368805,-0.001262590858708,0.0005298529188719,
   0.001852709827871,0.0006167695876579,-0.001743933399522,-0.001866740017442,
    0.00076490024494, 0.002626952193308,0.0008596990771097,-0.002387894351452,
  -0.002517617057674, 0.001015313090311, 0.003442529817681,  0.00111266313207,
  -0.003050449253802,-0.003179125184035, 0.001266593679849, 0.004250202618157,
   0.001359756564124,-0.003688187328674,-0.003806167811728, 0.001500886417515,
   0.004990271643667, 0.001581963360257,-0.004249933755065,-0.004346245625563,
   0.001697693337968, 0.005594984543238, 0.001757937015884,-0.004679017920573,
  -0.004741895202992, 0.001834864169188, 0.005991942160024,  0.00186513704711,
  -0.004916319691098,-0.004933895057517, 0.001889831164064, 0.006108358748425,
   0.001881221088503, -0.00490389552471,-0.004865030746555, 0.001841059619362,
   0.005875881350119, 0.001785542871583, -0.00458903074489,-0.004484135927301,
   0.001669664624059, 0.005235702678911, 0.001560692245144,-0.003928412581788,
  -0.003750244718356, 0.001360981326153, 0.004143628381928, 0.001194084572754,
   -0.00289209707472,-0.002636524487487,0.0009059909000529, 0.002574670929566,
  0.0006793444456889,-0.001466967201843,-0.001133595677547,0.0003025501751057,
  0.0005267807364227,1.738398303723e-05,0.0003405798813475,  0.00074798745888,
  -0.0004437266870343,-0.001976585277089,-0.0007829018296344, 0.002503011132788,
   0.002976366672885,-0.001319037007209,-0.004885410252884,-0.001704409278886,
   0.004971334427703, 0.005498323594271,-0.002301486861974,-0.008123728727903,
  -0.002722110033106, 0.007676159969192,  0.00824060771952,-0.003361709882838,
   -0.01159197236314,-0.003803884120837,  0.01053015304729,   0.0111126784871,
  -0.004463998975081, -0.01517100408925, -0.00491188403657,  0.01343179970738,
    0.01401073269743,-0.005567919816015, -0.01872766431965,-0.006004333460217,
    0.01627032002131,  0.01682280731161,-0.006630348621691, -0.02212151941592,
  -0.007037655083955,  0.01893142682886,  0.01943464014787,-0.007607761453886,
   -0.02521236080382,-0.007968817504302,  0.02130352949289,   0.0217359325773,
  -0.008458628023564, -0.02786800215564,-0.008757751741946,  0.02328403238375,
    0.02362661906899,-0.009145782178051, -0.02997187822763,-0.009369671159579,
    0.02478527965595,  0.02502269905808,-0.009638582395418, -0.03142987396692,
  -0.009777126672007,  0.02573971139425,  0.02586119429462,-0.009914730095889,
   -0.03217590905096,  -0.0099616250835,  0.02610390293843,  0.02610390293843,
    -0.0099616250835, -0.03217590905096,-0.009914730095889,  0.02586119429462,
    0.02573971139425,-0.009777126672007, -0.03142987396692,-0.009638582395418,
    0.02502269905808,  0.02478527965595,-0.009369671159579, -0.02997187822763,
  -0.009145782178051,  0.02362661906899,  0.02328403238375,-0.008757751741946,
   -0.02786800215564,-0.008458628023564,   0.0217359325773,  0.02130352949289,
  -0.007968817504302, -0.02521236080382,-0.007607761453886,  0.01943464014787,
    0.01893142682886,-0.007037655083955, -0.02212151941592,-0.006630348621691,
    0.01682280731161,  0.01627032002131,-0.006004333460217, -0.01872766431965,
  -0.005567919816015,  0.01401073269743,  0.01343179970738, -0.00491188403657,
   -0.01517100408925,-0.004463998975081,   0.0111126784871,  0.01053015304729,
  -0.003803884120837, -0.01159197236314,-0.003361709882838,  0.00824060771952,
   0.007676159969192,-0.002722110033106,-0.008123728727903,-0.002301486861974,
   0.005498323594271, 0.004971334427703,-0.001704409278886,-0.004885410252884,
  -0.001319037007209, 0.002976366672885, 0.002503011132788,-0.0007829018296344,
  -0.001976585277089,-0.0004437266870343,  0.00074798745888,0.0003405798813475,
  1.738398303723e-05,0.0005267807364227,0.0003025501751057,-0.001133595677547,
  -0.001466967201843,0.0006793444456889, 0.002574670929566,0.0009059909000529,
  -0.002636524487487, -0.00289209707472, 0.001194084572754, 0.004143628381928,
   0.001360981326153,-0.003750244718356,-0.003928412581788, 0.001560692245144,
   0.005235702678911, 0.001669664624059,-0.004484135927301, -0.00458903074489,
   0.001785542871583, 0.005875881350119, 0.001841059619362,-0.004865030746555,
   -0.00490389552471, 0.001881221088503, 0.006108358748425, 0.001889831164064,
  -0.004933895057517,-0.004916319691098,  0.00186513704711, 0.005991942160024,
   0.001834864169188,-0.004741895202992,-0.004679017920573, 0.001757937015884,
   0.005594984543238, 0.001697693337968,-0.004346245625563,-0.004249933755065,
   0.001581963360257, 0.004990271643667, 0.001500886417515,-0.003806167811728,
  -0.003688187328674, 0.001359756564124, 0.004250202618157, 0.001266593679849,
  -0.003179125184035,-0.003050449253802,  0.00111266313207, 0.003442529817681,
   0.001015313090311,-0.002517617057674,-0.002387894351452,0.0008596990771097,
   0.002626952193308,  0.00076490024494,-0.001866740017442,-0.001743933399522,
  0.0006167695876579, 0.001852709827871,0.0005298529188719,-0.001262590858708,
  -0.001152699368805,0.0003962072043767, 0.001157162142485,0.0003209210248809,
  -0.0007314272529938,-0.0006384303045448, 0.000206541029895,0.0005654166939372,
  0.0001451343680556,-0.0002896650877668,-0.0002156600448077,5.258399473393e-05,
  9.084769417943e-05,5.954819118623e-06, 5.54511957325e-05,0.0001100619362646,
  -6.41663240665e-05,-0.0002637057084412,-9.634528249988e-05,0.0003046044125928,
  0.0003408589216372,-0.0001450072079932,-0.0005040721510313,-0.0001640395580874,
   0.000464857450957,0.0004848413160558,-0.0001934536048555,-0.0006426752026104,
  -0.0002012316940175,0.0005479261554693,0.0005543764866565,-0.0002144989224296,
  -0.0006962682863811,-0.0002132927053908, 0.000568276521277,0.0005643192687603,
  -0.0002138516919861,-0.0006838020663353,-0.0002061474994892,0.0005415237605539,
  0.0005303135082512,-0.0001975475974528,-0.0006245171573531,-0.000185627683138,
  0.0004829964454818,0.0004673966463355,-0.0001712456510351,-0.0005363804029423,
  -0.0001572049173897,  0.00040649407191,0.0003890804726499,-0.0001398922945606,
  -0.0004349409006286,-0.0001254760228883,0.0003237039064647,0.0003064171372759,
  -0.0001075779696676,-0.0003325903960892,-9.398785474977e-05,0.0002435803737285,
  0.0002278909321653,-7.723146575159e-05,-0.0002383431214105,-6.540223339257e-05,
  0.0001723657362514,0.0001591173176174,-5.08446917726e-05,-0.0001579112012099,
  -4.11342835417e-05, 0.000113599637106,0.0001032497032984,-2.943363024564e-05,
  -9.394869692614e-05,-2.202267065696e-05,6.876685924578e-05,6.127583862425e-05,
  -1.315710194372e-05,-0.0001153864590388
};
