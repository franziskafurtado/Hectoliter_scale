import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';

import { IonicModule } from '@ionic/angular';

import { WaitPageRoutingModule } from './wait-routing.module';

import { WaitPage } from './wait.page';

@NgModule({
  imports: [
    CommonModule,
    FormsModule,
    IonicModule,
    WaitPageRoutingModule
  ],
  declarations: [WaitPage]
})
export class WaitPageModule {}
