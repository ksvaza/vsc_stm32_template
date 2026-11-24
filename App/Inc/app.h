/*
 * app.h
 *
 *  Created on: 24 Nov 2026
 *      Author: ksvaz
 */

#ifndef INC_APP_H_
#define INC_APP_H_

// Viss lietotāja kods dzīvo App/ mapē, CubeMX to nekad neaizskar,
// tāpēc .ioc var pārģenerēt jebkurā brīdī.

void App_Init(); // izsauc vienreiz, pēc visiem MX_*_Init()
void App_Loop(); // izsauc main() while (1) ciklā
void App_Error(); // izsauc Error_Handler(), ātri mirgo PC13, neatgriežas


#endif /* INC_APP_H_ */
