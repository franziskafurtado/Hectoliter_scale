import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { EndPage } from './end.page';

const routes: Routes = [
  {
    path: '',
    component: EndPage
  }
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule],
})
export class EndPageRoutingModule {}
