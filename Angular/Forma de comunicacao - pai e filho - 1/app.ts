import { Component } from '@angular/core';
import { NgIf } from '@angular/common';
import { RouterOutlet, Router, RouterLink, NavigationEnd } from '@angular/router';
import { Header } from "./components/header/header";
import { Footer } from "./components/footer/footer";

@Component({
  selector: 'app-root',
  imports: [RouterOutlet, RouterLink, Header, Footer],
  templateUrl: './app.html',
  styleUrl: './app.css'
})
export class App {
  protected title = 'plat-mecajun';
  menuAberto = false;

  toggleMenu() {
    this.menuAberto = !this.menuAberto;
  }

  urlAtual: string = '';

  constructor(private router: Router) {
    this.router.events.subscribe((event: any) => {
      this.urlAtual = event.urlAfterRedirects;
    });
  }
}
